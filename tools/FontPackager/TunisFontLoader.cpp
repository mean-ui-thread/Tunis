/*******************************************************************************
 * MIT License
 *
 * Copyright (c) 2017-2018 Mathieu-André Chiasson
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 * Disclaimer:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
#include "TunisFontLoader.h"

#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/URI.h>
#include <Poco/Base64Decoder.h>
#include <Poco/Glob.h>

#include <iostream>
#include <sstream>

#include <rapidjson/document.h>

using namespace tunis;

FontLoader::FontLoader()
{
    FT_Error error = FT_Init_FreeType(&m_library);
    if (error)
    {
        std::cerr << "Unable to init Freetype." << std::endl;
        abort();
    }
}

FontLoader::~FontLoader()
{
    for(FT_Face &face : m_faces)
    {
        FT_Done_Face(face);
    }

    for (char* data : m_faceData)
    {
        free(data);
    }

    FT_Done_FreeType(m_library);
}

void FontLoader::addFamily(const std::string &family)
{
   m_families.insert(family);
}

void FontLoader::addFilePattern(const std::string &pattern)
{
    m_patterns.insert(pattern);
}

const std::vector<FT_Face> &FontLoader::getFaces()
{
    for(FT_Face &face : m_faces)
    {
        FT_Done_Face(face);
    }
    m_faces.clear();

    loadFonts();
    loadWebFonts();

    return m_faces;
}

void FontLoader::loadFonts()
{
    std::set<std::string> files;
    for(const std::string &pattern : m_patterns)
    {
        Poco::Glob::glob(pattern, files);
    }

    for(const std::string &file : files)
    {
        FT_Face face;
        FT_Error error = FT_New_Face(m_library, file.c_str(), 0, &face);

        if (error)
        {
            std::cerr << "Could not open " << file << std::endl;
        }
        else
        {

            std::cout << "Loaded " << file << std::endl;
            m_faces.push_back(face);
        }
    }
}

void FontLoader::loadWebFonts()
{
    if (m_families.size() == 0)
    {
        return;
    }

    std::istringstream istr("aHR0cHM6Ly93d3cu"
                            "Z29vZ2xlYXBpcy5j"
                            "b20vd2ViZm9udHMv"
                            "djEvd2ViZm9udHM/"
                            "a2V5PUFJemFTeUQ2"
                            "NTlaNnlhOGlaN2dM"
                            "Qk04NlJHZHVmTVJJ"
                            "OFQxblpDQQ==");
    Poco::Base64Decoder decoder(istr);
    std::string result;
    decoder >> result;
    Poco::URI uri(std::move(result));

    const Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
    Poco::Net::HTTPResponse response;
    session.sendRequest(request);
    std::istream& is = session.receiveResponse(response);
    if (response.getStatus() == Poco::Net::HTTPServerResponse::HTTP_OK)
    {
        std::string jsonData;
        Poco::StreamCopier::copyToString(is, jsonData);

        rapidjson::Document document;
        document.Parse(jsonData.c_str());
        const rapidjson::Value &items = document["items"];
        for (rapidjson::SizeType itemId = 0; itemId < items.Size(); itemId++)
        {
            const rapidjson::Value &item = items[itemId];

            std::string category = item["category"].GetString();
            std::string family = item["family"].GetString();
            const rapidjson::Value &files = item["files"];

            bool valid = false;

            for(const std::string &validFamily: m_families)
            {
                valid = Poco::icompare(family, validFamily) == 0;
                if (valid) break;
            }

            if (!valid) { continue; }

            rapidjson::Value::ConstMemberIterator fileItr = files.MemberBegin(), fileEnd = files.MemberEnd();
            while (fileItr != fileEnd)
            {
                Poco::URI fontUrl(fileItr->value.GetString());

                Poco::Net::HTTPClientSession fontDownloadSession(fontUrl.getHost(), fontUrl.getPort());
                Poco::Net::HTTPRequest fontDownloadRequest(Poco::Net::HTTPRequest::HTTP_GET, fontUrl.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
                Poco::Net::HTTPResponse fontDownloadResponse;
                fontDownloadSession.sendRequest(fontDownloadRequest);
                std::istream& is = fontDownloadSession.receiveResponse(fontDownloadResponse);

                size_t length = 0;
                char * data = nullptr;

                while (is.good())
                {
                    const size_t blocksize = 4096;
                    data = reinterpret_cast<char*>(realloc(data, length + blocksize));
                    is.read(&data[length], blocksize);
                    length += static_cast<size_t>(is.gcount());
                    data = reinterpret_cast<char*>(realloc(data, length));
                }

                m_faceData.push_back(data);

                FT_Face face;
                FT_Error error = FT_New_Memory_Face(m_library, reinterpret_cast<const uint8_t*>(data), static_cast<FT_Long>(length), 0, &face);

                if (error)
                {
                    std::cerr << "Could not open " << fileItr->value.GetString() << std::endl;
                }
                else
                {
                    std::cout << "Loaded " << face->family_name << " " << face->style_name << std::endl;
                    m_faces.push_back(face);
                }

                ++fileItr;
            }
        }
    }
}

