/*
This program renders a font formatted as C source as a PNG file. It's extremely
brittle, so don't expect much out of it.  The input format expected by the
program is like:

0x12, 0x34, 0x56, 0x78, 0x9A,
...
0x12, 0x34, 0x56, 0x78, 0x9A,
<EOF>

with exactly five bytes per line, no leading or trailing spaces, or any other
lines or characters except for the font data itself. Creates an image called
"font.png" in the working directory.

Usage:
 $ go run font2png.go [-scale <n>] < glcdfont.c

Options:
 -scale <n>  The pixel scale. Set to greater than 1 to scale the output image
             by an integer for easier viewing.

*/

package main

import (
	"flag"
	"fmt"
	"image"
	"image/color"
	"image/draw"
	"image/png"
	"log"
	"os"
)

const (
	W = 5
	H = 8
	N = 255
)

var scale int

func init() {
	flagScale := flag.Int("scale", 5, "pixel scale")
	flag.Parse()
	scale = *flagScale
}

func main() {
	var b [5]byte
	p := image.NewGray(image.Rect(0, 0, 16*(W+1)*scale, 16*H*scale))

	for i := 0; ; i++ {
		n, err := fmt.Scanf("0x%X, 0x%X, 0x%X, 0x%X, 0x%X,\n", &b[0], &b[1], &b[2], &b[3], &b[4])
		if err != nil {
			if n == 0 {
				break
			}
			log.Fatal(err)
		}

		for x := 0; x < W; x++ {
			for y := 0; y < H; y++ {
				set(p, x, y, i, coords(b, x, y))
				//p.Set(x+i*(W+1), y, coords(b, x, y))
			}
		}
		for y := 0; y < H; y++ {
			set(p, W, y, i, color.White)
			//p.Set(, y, color.White)
		}
	}

	f, err := os.Create("font.png")
	if err != nil {
		log.Fatal(err)
	}
	png.Encode(f, p)
	f.Close()
}

func coords(p [5]byte, x, y int) color.Color {
	n := x*8 + y
	i := n / 8
	if (p[i]>>uint(y))&1 > 0 {
		return color.Black
	}
	return color.White
}

func set(p draw.Image, x, y, i int, c color.Color) {
	var r image.Rectangle
	r.Min.Y = i / 16
	r.Min.X = i - r.Min.Y*16

	r.Min.X = (r.Min.X*(W+1) + x) * scale
	r.Min.Y = (r.Min.Y*H + y) * scale
	r.Max.X = r.Min.X + scale
	r.Max.Y = r.Min.Y + scale
	u := image.NewUniform(c)
	draw.Draw(p, r, u, image.ZP, draw.Src)
}
