/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "ToyGine2", "index.html", [
    [ "Supported Systems", "index.html#supported-systems", [
      [ "Supported C++ compilers", "index.html#supported-c-compilers", null ],
      [ "Tools", "index.html#tools", null ]
    ] ],
    [ "AGENTS Guidelines for This Repository", "df/d81/md__a_g_e_n_t_s.html", [
      [ "Interaction Guidelines", "df/d81/md__a_g_e_n_t_s.html#interaction-guidelines", null ],
      [ "Project Structure", "df/d81/md__a_g_e_n_t_s.html#project-structure", null ],
      [ "C++ game engine style guide", "df/d81/md__a_g_e_n_t_s.html#c-game-engine-style-guide", null ],
      [ "Project Context", "df/d81/md__a_g_e_n_t_s.html#project-context", null ],
      [ "General Coding Principles", "df/d81/md__a_g_e_n_t_s.html#general-coding-principles", null ],
      [ "Language and Standard", "df/d81/md__a_g_e_n_t_s.html#language-and-standard", null ],
      [ "Formatting", "df/d81/md__a_g_e_n_t_s.html#formatting", [
        [ "Indentation and Spacing", "df/d81/md__a_g_e_n_t_s.html#indentation-and-spacing", null ],
        [ "Braces", "df/d81/md__a_g_e_n_t_s.html#braces", null ],
        [ "Pointer and Reference Alignment", "df/d81/md__a_g_e_n_t_s.html#pointer-and-reference-alignment", null ],
        [ "Binary Operators", "df/d81/md__a_g_e_n_t_s.html#binary-operators", null ],
        [ "Empty Lines", "df/d81/md__a_g_e_n_t_s.html#empty-lines", null ]
      ] ],
      [ "Header / Source Organization", "df/d81/md__a_g_e_n_t_s.html#header--source-organization", [
        [ "Module Structure", "df/d81/md__a_g_e_n_t_s.html#module-structure", null ],
        [ "Include Guards", "df/d81/md__a_g_e_n_t_s.html#include-guards", null ],
        [ "File Extensions", "df/d81/md__a_g_e_n_t_s.html#file-extensions", null ],
        [ "Include Order", "df/d81/md__a_g_e_n_t_s.html#include-order", null ],
        [ "Headers", "df/d81/md__a_g_e_n_t_s.html#headers", null ],
        [ "Inline and Template Code", "df/d81/md__a_g_e_n_t_s.html#inline-and-template-code", null ],
        [ "<span class=\"tt\">.inl</span> Files", "df/d81/md__a_g_e_n_t_s.html#inl-files", null ],
        [ "Access Modifiers", "df/d81/md__a_g_e_n_t_s.html#access-modifiers", null ]
      ] ],
      [ "Memory and Allocation Rules", "df/d81/md__a_g_e_n_t_s.html#memory-and-allocation-rules", null ],
      [ "Constexpr-First Design", "df/d81/md__a_g_e_n_t_s.html#constexpr-first-design", null ],
      [ "Modern C++ Practices", "df/d81/md__a_g_e_n_t_s.html#modern-c-practices", [
        [ "noexcept", "df/d81/md__a_g_e_n_t_s.html#noexcept", null ],
        [ "<span class=\"tt\">[[nodiscard]]</span>", "df/d81/md__a_g_e_n_t_s.html#nodiscard", null ],
        [ "<span class=\"tt\">using</span> over <span class=\"tt\">typedef</span>", "df/d81/md__a_g_e_n_t_s.html#using-over-typedef", null ],
        [ "Explicit Constructors", "df/d81/md__a_g_e_n_t_s.html#explicit-constructors", null ],
        [ "<span class=\"tt\">= default</span> and <span class=\"tt\">= delete</span>", "df/d81/md__a_g_e_n_t_s.html#autotoc_md-default-and--delete", null ],
        [ "Rule of Zero and Rule of Five", "df/d81/md__a_g_e_n_t_s.html#rule-of-zero-and-rule-of-five", null ],
        [ "<span class=\"tt\">auto</span>", "df/d81/md__a_g_e_n_t_s.html#auto", null ],
        [ "Range-Based For Loops", "df/d81/md__a_g_e_n_t_s.html#range-based-for-loops", null ],
        [ "Default Member Initialization", "df/d81/md__a_g_e_n_t_s.html#default-member-initialization", null ]
      ] ],
      [ "Error Handling", "df/d81/md__a_g_e_n_t_s.html#error-handling", [
        [ "Assertions", "df/d81/md__a_g_e_n_t_s.html#assertions", null ]
      ] ],
      [ "Naming Conventions", "df/d81/md__a_g_e_n_t_s.html#naming-conventions", [
        [ "STL-Compatible Naming", "df/d81/md__a_g_e_n_t_s.html#stl-compatible-naming", null ],
        [ "Private Members", "df/d81/md__a_g_e_n_t_s.html#private-members", null ],
        [ "Const Reference Style", "df/d81/md__a_g_e_n_t_s.html#const-reference-style", null ]
      ] ],
      [ "Comments and Documentation", "df/d81/md__a_g_e_n_t_s.html#comments-and-documentation", [
        [ "Documentation Tone", "df/d81/md__a_g_e_n_t_s.html#documentation-tone", null ],
        [ "Doxygen Block Style", "df/d81/md__a_g_e_n_t_s.html#doxygen-block-style", null ]
      ] ],
      [ "Documentation Style Rules", "df/d81/md__a_g_e_n_t_s.html#documentation-style-rules", [
        [ "File documentation (<span class=\"tt\">\\file</span>)", "df/d81/md__a_g_e_n_t_s.html#file-documentation-file", [
          [ "Template: public header (<span class=\"tt\">.hpp</span>)", "df/d81/md__a_g_e_n_t_s.html#template-public-header-hpp", null ],
          [ "Template: implementation (<span class=\"tt\">.cpp</span>)", "df/d81/md__a_g_e_n_t_s.html#template-implementation-cpp", null ],
          [ "Template: inline implementation (<span class=\"tt\">.inl</span>)", "df/d81/md__a_g_e_n_t_s.html#template-inline-implementation-inl", null ]
        ] ],
        [ "Method / Function Documentation Order", "df/d81/md__a_g_e_n_t_s.html#method--function-documentation-order", null ],
        [ "Class / Struct Documentation Order", "df/d81/md__a_g_e_n_t_s.html#class--struct-documentation-order", null ],
        [ "Concept Documentation", "df/d81/md__a_g_e_n_t_s.html#concept-documentation", [
          [ "<span class=\"tt\">\\section performance Performance Characteristics</span>", "df/d81/md__a_g_e_n_t_s.html#section-performance-performance-characteristics", null ],
          [ "<span class=\"tt\">\\section safety Safety Guarantees</span>", "df/d81/md__a_g_e_n_t_s.html#section-safety-safety-guarantees", null ],
          [ "<span class=\"tt\">\\section compatibility Compatibility</span> (optional)", "df/d81/md__a_g_e_n_t_s.html#section-compatibility-compatibility-optional", null ]
        ] ]
      ] ],
      [ "Class Documentation Templates", "df/d81/md__a_g_e_n_t_s.html#class-documentation-templates", [
        [ "Regular Class", "df/d81/md__a_g_e_n_t_s.html#regular-class", null ],
        [ "Concept", "df/d81/md__a_g_e_n_t_s.html#concept", null ],
        [ "Template Class", "df/d81/md__a_g_e_n_t_s.html#template-class", null ],
        [ "Struct", "df/d81/md__a_g_e_n_t_s.html#struct", null ],
        [ "Simplified Class (without compatibility)", "df/d81/md__a_g_e_n_t_s.html#simplified-class-without-compatibility", null ],
        [ "Documentation Pre-Commit Checklist", "df/d81/md__a_g_e_n_t_s.html#documentation-pre-commit-checklist", null ]
      ] ],
      [ "Unit Test Style Rules", "df/d81/md__a_g_e_n_t_s.html#unit-test-style-rules", null ],
      [ "Test Framework Assumptions", "df/d81/md__a_g_e_n_t_s.html#test-framework-assumptions", null ],
      [ "General Test Principles", "df/d81/md__a_g_e_n_t_s.html#general-test-principles", null ],
      [ "Test Length and Structure", "df/d81/md__a_g_e_n_t_s.html#test-length-and-structure", null ],
      [ "Compile-Time vs Runtime Testing", "df/d81/md__a_g_e_n_t_s.html#compile-time-vs-runtime-testing", [
        [ "Compile-Time (<span class=\"tt\">constexpr</span>) Tests", "df/d81/md__a_g_e_n_t_s.html#compile-time-constexpr-tests", null ],
        [ "Runtime Tests", "df/d81/md__a_g_e_n_t_s.html#runtime-tests", null ]
      ] ],
      [ "Redundancy and Duplication", "df/d81/md__a_g_e_n_t_s.html#redundancy-and-duplication", null ],
      [ "Value-Based Assertions", "df/d81/md__a_g_e_n_t_s.html#value-based-assertions", null ],
      [ "String length assertions (including UTF-8)", "df/d81/md__a_g_e_n_t_s.html#string-length-assertions-including-utf-8", null ],
      [ "Constexpr + Runtime Parity", "df/d81/md__a_g_e_n_t_s.html#constexpr--runtime-parity", null ],
      [ "Naming Tests", "df/d81/md__a_g_e_n_t_s.html#naming-tests", null ],
      [ "Floating-Point Rules", "df/d81/md__a_g_e_n_t_s.html#floating-point-rules", null ],
      [ "Test Independence", "df/d81/md__a_g_e_n_t_s.html#test-independence", null ],
      [ "Final Rule", "df/d81/md__a_g_e_n_t_s.html#final-rule", null ],
      [ "Tools and Automation", "df/d81/md__a_g_e_n_t_s.html#tools-and-automation", null ]
    ] ],
    [ "Topics", "topics.html", "topics" ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Concepts", "concepts.html", "concepts" ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"d3/d7a/classtoy_1_1_fixed_string.html#a59abe3a985749d133aa72a8ab3c92575",
"d4/da9/namespacetoy.html#acf7380257d9b8534ca8c156d8063e552",
"d7/d1d/classtoy_1_1application_1_1_core_application.html#a16c14348457b85c7dd6902a1133ec2ee",
"d8/de4/namespacetoy_1_1anonymous__namespace_02format_8inl_03.html#af5a101da1b38edd3c4d02acf12441202",
"dd/d5c/classtoy_1_1math_1_1fixed.html#a9565f03b6e9d30e7a2c8b499fd33ebf0",
"dir_68267d1309a1af8e8297ef4c3efbcdba.html"
];

var SYNCONMSG = 'click to disable panel synchronization';
var SYNCOFFMSG = 'click to enable panel synchronization';
var LISTOFALLMEMBERS = 'List of all members';