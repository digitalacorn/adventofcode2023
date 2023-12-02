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

func Reverse(s string) string {
	runes := []rune(s)
	for i, j := 0, len(runes)-1; i < j; i, j = i+1, j-1 {
		runes[i], runes[j] = runes[j], runes[i]
	}
	return string(runes)
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

	digitNames := [9]string{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}
	replacerArgs := make([]string, len(digitNames)*2)
	replacerArgsReverse := make([]string, len(digitNames)*2)

	for i, v := range digitNames {
		replacerArgs[i*2] = v
		replacerArgsReverse[i*2] = Reverse(v)
		replacerArgs[i*2+1] = fmt.Sprintf("%d", i+1)
		replacerArgsReverse[i*2+1] = fmt.Sprintf("%d", i+1)
	}

	wordReplacer := strings.NewReplacer(replacerArgs...)
	wordReplacerReverse := strings.NewReplacer(replacerArgsReverse...)

	for i := 0; i < len(lines); i++ {
		line := lines[i]
		forwardLine := wordReplacer.Replace(line)
		reverseLine := wordReplacerReverse.Replace(Reverse(line))
		numerics := regexp.MustCompile("[1-9]").FindAllString(forwardLine, -1)
		numericsReverse := regexp.MustCompile("[1-9]").FindAllString(reverseLine, -1)

		if len(numerics) < 1 {
			panic("no numbers found")
		}
		if len(numericsReverse) < 1 {
			panic("no numbers found")
		}
		first := []byte(numerics[0])[0]
		last := []byte(numericsReverse[0])[0]
		zero := []byte("0")[0]
		calibrationValue := (first-zero)*10 + last - zero

		fmt.Println("calibrationValue: ", calibrationValue, line)

		sum += int(calibrationValue)
	}

	fmt.Println("Total ", sum)
}
