#-------------------------------------------------------------------------------------------
# Copyright (c) 2025 by Toyman Interactive
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the "Software"), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and / or sell copies of the Software, and to permit
# persons to whom the Software is furnished to do so, subject to the following conditions :
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#-------------------------------------------------------------------------------------------

find_package(Doxygen REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

include(FetchContent)

FetchContent_Declare(doxygen_awesome_css GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git GIT_TAG v2.3.4)
FetchContent_MakeAvailable(doxygen_awesome_css)
FetchContent_GetProperties(doxygen_awesome_css SOURCE_DIR AWESOME_CSS_DIR)

FetchContent_Declare(m_css GIT_REPOSITORY https://github.com/mosra/m.css.git)
FetchContent_MakeAvailable(m_css)
FetchContent_GetProperties(m_css SOURCE_DIR M_CSS_DIR)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_awesome_css.in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_awesome_css @ONLY)

add_custom_target(docs
COMMENT "Generating documentation with Doxygen"
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_awesome_css
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_m_css
    COMMAND ${Python3_EXECUTABLE} ${M_CSS_DIR}/documentation/doxygen.py --no-doxygen --templates ${M_CSS_DIR}/documentation/templates/ --output ${CMAKE_CURRENT_SOURCE_DIR}/html/m xml/index.xml
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    VERBATIM
)
