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
    m_subsets.insert("latin");
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

    FT_Done_FreeType(m_library);
}

void FontLoader::addCategory(const std::string &category)
{
    m_categories.insert(category);
}

void FontLoader::addSubset(const std::string &subset)
{
    m_subsets.insert(subset);
}

void FontLoader::addFamily(const std::string &family)
{
   m_families.insert(family);
}

void FontLoader::addStyle(const std::string &styleStr)
{
    FontStyle style = FontStyle::GetStyleByName(styleStr);
    if (style == FontStyle::Invalid)
    {
        std::cerr << "Unsupported style " << styleStr << ". Supported styles are:\n";

        std::cerr << "\t\"Thin\",               \"100\",\n";
        std::cerr << "\t\"Thin Italic\",        \"100italic\",\n";
        std::cerr << "\t\"Extra-Light\",        \"200\",\n";
        std::cerr << "\t\"Extra-Light Italic\", \"200italic\",\n";
        std::cerr << "\t\"Light\",              \"300\",\n";
        std::cerr << "\t\"Light Italic\",       \"300italic\",\n";
        std::cerr << "\t\"Regular\",            \"regular\",\n";
        std::cerr << "\t\"Regular Italic\",     \"italic\",\n";
        std::cerr << "\t\"Medium\",             \"500\",\n";
        std::cerr << "\t\"Medium Italic\",      \"500italic\",\n";
        std::cerr << "\t\"Semi-Bold\",          \"600\",\n";
        std::cerr << "\t\"Semi-Bold Italic\",   \"600italic\",\n";
        std::cerr << "\t\"Bold\",               \"700\",\n";
        std::cerr << "\t\"Bold Italic\",        \"700italic\",\n";
        std::cerr << "\t\"Extra-Bold\",         \"800\",\n";
        std::cerr << "\t\"Extra-Bold Italic\",  \"800italic\",\n";
        std::cerr << "\t\"Black\",              \"900\",\n";
        std::cerr << "\t\"Black Italic\",       \"900italic\"" << std::endl;
    }
    else
    {
        m_styles.insert(style);
    }
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
    result += "&sort=popularity";
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
            const rapidjson::Value &subsets = item["subsets"];
            const rapidjson::Value &variants = item["variants"];
            const rapidjson::Value &files = item["files"];


            bool valid = false;

            for(const std::string &validSubset: m_subsets)
            {
                for (rapidjson::SizeType subsetId = 0; subsetId < subsets.Size(); subsetId++)
                {
                    std::string subset = subsets[subsetId].GetString();
                    valid = Poco::icompare(subset, validSubset) == 0;
                    if (valid) break;
                }

                if (valid) break;
            }

            if (!valid) { continue; }


            valid = m_categories.size() == 0;

            for(const std::string &validCategory: m_categories)
            {
                valid = Poco::icompare(category, validCategory) == 0;
                if (valid) break;
            }

            if (!valid) { continue; }

            valid = m_families.size() == 0;

            for(const std::string &validFamily: m_families)
            {
                valid = Poco::icompare(family, validFamily) == 0;
                if (valid) break;
            }

            if (!valid) { continue; }

            valid = m_styles.size() == 0;
            for(const FontStyle &validStyle: m_styles)
            {
                for (rapidjson::SizeType variantId = 0; variantId < variants.Size(); variantId++)
                {
                    FontStyle style = FontStyle::GetStyleByName(variants[variantId].GetString());

                    if (style == FontStyle::Invalid)
                    {
                        std::cerr << "Invalid Style " << variants[variantId].GetString() << std::endl;
                    }

                    valid = style == validStyle;
                    if (valid) break;
                }
                if (valid) break;
            }

            if (!valid) { continue; }

            rapidjson::Value::ConstMemberIterator fileItr = files.MemberBegin(), fileEnd = files.MemberEnd();
            while (fileItr != fileEnd)
            {
                Poco::URI fontUrl(fileItr->value.GetString());

                Poco::Net::HTTPSClientSession fontDownloadSession(fontUrl.getHost(), fontUrl.getPort(), context);
                Poco::Net::HTTPRequest fontDownloadRequest(Poco::Net::HTTPRequest::HTTP_GET, fontUrl.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
                Poco::Net::HTTPResponse fontDownloadResponse;
                fontDownloadSession.sendRequest(fontDownloadRequest);
                std::istream& is = fontDownloadSession.receiveResponse(fontDownloadResponse);

                std::vector<char> data;

                while (is.good())
                {
                    const size_t blocksize = 4096;
                    size_t oldsize = data.size();
                    data.resize(oldsize + blocksize);
                    is.read(&data[oldsize], blocksize);
                    data.resize(oldsize + static_cast<size_t>(is.gcount()));
                }

                FT_Face face;
                FT_Error error = FT_New_Memory_Face(m_library, reinterpret_cast<const uint8_t*>(data.data()), static_cast<FT_Long>(data.size()), 0, &face);

                if (error)
                {
                    std::cerr << "Could not open " << fileItr->value.GetString() << std::endl;
                }
                else
                {
                    std::cout << "Loaded " << fileItr->value.GetString() << std::endl;
                    m_faces.push_back(face);
                }

                ++fileItr;
            }
        }
    }
}

