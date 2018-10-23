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

#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/HelpFormatter.h>

#include <iostream>

#include "TunisFontGenerator.h"
#include "TunisFontLoader.h"

namespace tunis
{
    class FontPackager : public Poco::Util::Application
    {
        FontGenerator m_generator;
        FontLoader m_loader;
        std::string m_output = "fonts.tfp";
        bool m_helpRequested = false;

    public:
        FontPackager()
        {
            setUnixOptions(true);
        }

        void defineOptions(Poco::Util::OptionSet &options)
        {
            Application::defineOptions(options);

            options.addOption(Poco::Util::Option("help", "h", "Usage Information.")
                        .required(false)
                        .repeatable(false)
                        .callback(Poco::Util::OptionCallback<FontPackager>(this, &FontPackager::handleHelp)));

            options.addOption(Poco::Util::Option("output", "o", "Font package file output.  Default is 'font.tfp'.")
                        .required(false)
                        .repeatable(false)
                        .callback(Poco::Util::OptionCallback<FontPackager>(this, &FontPackager::handleOutput)));

            options.addOption(Poco::Util::Option("font", "f", "Font file pattern to add to the package. Wildcard (*) allowed.")
                        .required(false)
                        .repeatable(true)
                        .argument("pattern")
                        .callback(Poco::Util::OptionCallback<FontPackager>(this, &FontPackager::handleFont)));

            options.addOption(Poco::Util::Option("webfont", "w", "Online Font to add to the package.")
                        .required(false)
                        .repeatable(true)
                        .argument("family")
                        .callback(Poco::Util::OptionCallback<FontPackager>(this, &FontPackager::handleWebFont)));
        }

        void displayHelp()
        {
            Poco::Util::HelpFormatter helpFormatter(options());
            helpFormatter.setCommand(commandName());
            helpFormatter.setHeader("Tunis Font Packager.\nCopyright (c) 2015-2018 Mathieu-Andre Chiasson\nAll rights reserved.\n\nTurns group of font into a Tunis font Package (.tfp)");
            helpFormatter.setUsage("[-f <pattern1> [-f <pattern2> [...]]] [-w family1 [-w family2 [...]]]");
            helpFormatter.format(std::cout);
        }

        void handleHelp(const std::string& name, const std::string& value)
        {
            m_helpRequested = true;
            stopOptionsProcessing();
        }

        void handleOutput(const std::string& name, const std::string& value)
        {
            m_output = value;
        }

        void handleFont(const std::string& name, const std::string& value)
        {
            m_loader.addFilePattern(value);
        }

        void handleWebFont(const std::string& name, const std::string& value)
        {
            m_loader.addFamily(value);
        }

        int main(const std::vector<std::string>& args)
        {
            if (m_helpRequested)
            {
                displayHelp();
                return EXIT_OK;
            }

            const std::vector<FT_Face> &faces = m_loader.getFaces();

            if (faces.size() == 0)
            {
                std::cerr << "Unable to load any fonts." << std::endl;
                displayHelp();
                return EXIT_USAGE;
            }

            m_generator.generate(m_output, faces);

            return EXIT_OK;
        }
    };
}

POCO_APP_MAIN(tunis::FontPackager)
