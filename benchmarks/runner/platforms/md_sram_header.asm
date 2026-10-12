;-----------------------------------------------------------------------------------------------------------------------
; Copyright (c) 2026 Toyman Interactive
;
; Permission is hereby granted, free of charge, to any person obtaining a copy of this
; software and associated documentation files (the "Software"), to deal in the Software
; without restriction, including without limitation the rights to use, copy, modify, merge,
; publish, distribute, sublicense, and / or sell copies of the Software, and to permit
; persons to whom the Software is furnished to do so, subject to the following conditions :
;
; The above copyright notice and this permission notice shall be included in all copies or
; substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
; INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
; PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
; FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
; OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
; DEALINGS IN THE SOFTWARE.
;-----------------------------------------------------------------------------------------------------------------------

; Rebuilds rom.bin with cartridge RAM declared in the header the ClownMDSDK crt0 leaves blank: 8-bit RAM on odd
; addresses from 0x200001 to 0x20FFFF. md_sram_header.cmake assembles it with rom.bin's directory on the include path.

        cpu      68000
        org      0
        binclude "rom.bin",0,$1B0
        dc.b     "RA",$F8,$20
        dc.l     $200001,$20FFFF
        binclude "rom.bin",$1BC
