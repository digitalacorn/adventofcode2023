package main

import (
	"fmt"
	"os"
	"strings"
)

func Reverse(s string) string {
	runes := []rune(s)
	for i, j := 0, len(runes)-1; i < j; i, j = i+1, j-1 {
		runes[i], runes[j] = runes[j], runes[i]
	}
	return string(runes)
}

func FirstNumeric(s string) int {
	runes := []rune(s)
	zero := ([]rune("0"))[0]
	nine := ([]rune("9"))[0]
	for i := 0; i < len(runes); i++ {
		if runes[i] >= zero && runes[i] <= nine {
			return int(runes[i] - zero)
		}
	}
	panic("No digit in string")
}

func main() {
	args := os.Args[1:]
	if len(args) != 1 {
		panic("Incorrect args count")
	}
	inputFileName := args[0]
	fmt.Println("Reading", inputFileName)
	data, err := os.ReadFile(inputFileName)
	if err != nil {
		panic(err)
	}
	lines := strings.Split(string(data), "\n")
	sum := 0
	fmt.Println("Read", len(lines), "lines")

	// NewReplacer takes args like ("one", "1", "two", "2") but will replace strings from left to right
	// But we need to replace words from right to left when finding the last numeric word in a string
	// so we reverse the strings before replacing to find the last digit
	// e.g. fiveightwone
	// replacing left to right yields '5igh2ne', and right to left yields 'fiv8w1'
	// the corrrect callibration value is 51 (1st digit replaces left to right, last digit replaced right to left)
	//
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
		calibrationValue := FirstNumeric(forwardLine)*10 + FirstNumeric(reverseLine)

		fmt.Println("calibrationValue: ", calibrationValue, line)

		sum += int(calibrationValue)
	}

	fmt.Println("Total ", sum)
}
