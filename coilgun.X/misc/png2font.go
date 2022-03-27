// This program converts a 1:1 scale black and white PNG to bytes representing a
// bitmap font in the format accepted by the font C struct.
//
// Usage:
//  $ go run png2font.go < font.png

package main

import (
	"fmt"
	"image/color"
	"image/png"
	"log"
	"os"
	"strings"
	"unicode"
)

const (
	CH_W = 6 // char width in pixels (bits)
	CH_H = 8 // char height in pixels (bits)
)

func main() {
	log.SetFlags(0)
	img, err := png.Decode(os.Stdin)
	if err != nil {
		log.Fatal(err)
	}

	w := img.Bounds().Dx()
	h := img.Bounds().Dy()
	if w%CH_W != 0 || h%CH_H != 0 {
		log.Fatal("bad dimensions")
	}

	log.Println((w*h)/(CH_W*CH_H), "chars")

	buf := make([]uint8, (w*h)/8)
	i := 0

	for y := 0; y < h; y += CH_H {
		for x := 0; x < w; x += CH_W {
			for xx := 0; xx < CH_W; xx++ {
				for yy := 0; yy < CH_H; yy++ {
					c := color.Gray16Model.Convert(img.At(x+xx, y+yy))
					switch c {
					case color.Black:
						buf[i] |= 1 << uint(yy)
					case color.White:
						buf[i] &= ^(1 << uint(yy))
					}
				}
				i++
			}
		}
	}

	// cut off the empty space at the bottom of the char (inserted in pic for visual separation only)
	s := make([]string, CH_W)

	for i := 0; i < len(buf); i += CH_W {
		a := buf[i : i+CH_W]
		for j, col := range a {
			s[j] = fmt.Sprintf("0x%02X", col)
		}
		ss := strings.Join(s, ", ")
		if i < (len(buf) - 2) {
			ss += ","
		}
		ss += fmt.Sprintf(" // 0x%02X", i/CH_W)
		ch := rune(i / CH_W)
		if unicode.IsPrint(ch) {
			ss += fmt.Sprintf(" %c", ch)
		}
		fmt.Println(ss)
	}
}
