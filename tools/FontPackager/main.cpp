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

#include "TunisFontLoader.h"

namespace tunis
{
    class FontPackager : public Poco::Util::Application
    {
        bool m_helpRequested = false;
        FontLoader m_loader;

    public:
        FontPackager()
        {
            setUnixOptions(true);
        }

        void defineOptions(Poco::Util::OptionSet &options)
        {
            Application::defineOptions(options);

            options.addOption(Poco::Util::Option("help", "h", "Usage Information. ")
                        .required(false)
                        .repeatable(false)
                        .callback(Poco::Util::OptionCallback<FontPackager>(this, &FontPackager::handleHelp)));

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

            options.addOption(Poco::Util::Option("language", "l", "Add extra language subsets to package. 'latin' is included by default.")
                        .required(false)
                        .repeatable(true)
                        .argument("lang")
                        .callback(Poco::Util::OptionCallback<FontPackager>(this, &FontPackager::handleSubset)));

            options.addOption(Poco::Util::Option("style", "s", "restrict styles to package. every styles are included by default until at least one style is provided.")
                        .required(false)
                        .repeatable(true)
                        .argument("style")
                        .callback(Poco::Util::OptionCallback<FontPackager>(this, &FontPackager::handleStyle)));

        }

        void displayHelp()
        {
            Poco::Util::HelpFormatter helpFormatter(options());
            helpFormatter.setCommand(commandName());
            helpFormatter.setUsage("[-f <font file> [-f <font file> [...]]]");
            helpFormatter.setHeader("Tunis Font Packager. Turns group of font into a Tunis font Package (.tfp)");
            helpFormatter.format(std::cout);
        }

        void handleHelp(const std::string& name, const std::string& value)
        {
            m_helpRequested = true;
            stopOptionsProcessing();
        }

        void handleFont(const std::string& name, const std::string& value)
        {
            m_loader.addFilePattern(value);
        }

        void handleWebFont(const std::string& name, const std::string& value)
        {
            m_loader.addFamily(value);
        }

        void handleSubset(const std::string& name, const std::string& value)
        {
            m_loader.addSubset(value);
        }

        void handleStyle(const std::string& name, const std::string& value)
        {
            m_loader.addStyle(value);
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

            return EXIT_OK;
        }
    };
}

POCO_APP_MAIN(tunis::FontPackager)
