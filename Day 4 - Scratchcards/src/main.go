package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

func ReadFirstArg() []string {
	absPath, _ := filepath.Abs(os.Args[1])
	file, err := os.Open(absPath)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	var t []string
	for scanner.Scan() {
		t = append(t, scanner.Text())
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	return t
}

func main() {
	lines := ReadFirstArg()

	for i := 0; i < len(lines); i++ {
		e := lines[i]

		cardNum, err := strconv.Atoi(e[strings.IndexAny(e, "1234567890"):strings.Index(e, ":")])
		if err != nil {
			log.Fatal(err)
		}

		e = e[strings.Index(e, ":")+2:]
		e = strings.ReplaceAll(e, "  ", " ")

		game := strings.Split(e, "|")

		winningNums := strings.Split(strings.Trim(game[0], " "), " ")
		possessedNums := strings.Split(strings.Trim(game[1], " "), " ")

		intersection := Intersection(winningNums, possessedNums)

		matches := len(intersection)
		for i := 0; i < int(matches); i++ {
			fmt.Println("Copied Card:", cardNum+i)
			lines = append(lines, lines[cardNum+i])
		}

		fmt.Println("Card:", cardNum)
		fmt.Println("Winning numbers:", winningNums)
		fmt.Println("Possessed numbers:", possessedNums)
		fmt.Println("Intersected numbers:", intersection)
		fmt.Println("Number of Intersections:", len(intersection))
		fmt.Println("Points:", matches)

		fmt.Println()
	}

	fmt.Println("All Scratchcards:", len(lines))
}

// Simple has complexity: O(n^2)
func Intersection[T comparable](a []T, b []T) []T {
	set := make([]T, 0)

	for _, v := range a {
		if containsGeneric(b, v) {
			set = append(set, v)
		}
	}

	return set
}

func containsGeneric[T comparable](b []T, e T) bool {
	for _, v := range b {
		if v == e {
			return true
		}
	}
	return false
}
