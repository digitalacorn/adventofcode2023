package main

import (
	"fmt"
	"os"
	"regexp"
	"strings"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	args := os.Args[1:]
	if len(args) != 1 {
		panic("Incorrect args count")
	}
	inputFileName := args[0]
	fmt.Println("Reading", inputFileName)
	data, err := os.ReadFile(inputFileName)
	check(err)
	lines := strings.Split(string(data), "\n")
	sum := 0
	fmt.Println("Read", len(lines), "lines")

	for i := 0; i < len(lines); i++ {
		line := lines[i]
		numerics := regexp.MustCompile("[0-9]").FindAllString(line, -1)
		if len(numerics) < 1 {
			panic("no numbers found")
		}
		first := []byte(numerics[0])[0]
		last := []byte(numerics[len(numerics)-1])[0]
		zero := []byte("0")[0]
		calibrationValue := (first-zero)*10 + last - zero
		sum += int(calibrationValue)
	}

	fmt.Println("Total ", sum)

}
