# sudoku-generator

Generates minimal unambiguous sudokus and their solution.
The unsolved sudokus are printed to stdout, solutions are printed to stderr.

## Usage:
There are two optional arguments, both have to be integers:

The first argument is the number of sudokus generated, the second number is the offset for the label.
For example,

```bash
./sudoku-generator 3 42 1>your_sudoku_file.txt 2>your_solution_file.txt
```
prints 3 sudokus, labeled 42, 43 and 44, to your_sudoku_file.txt and their solution to your_solution_file.txt.

The default values are 1 for both variables.

## Printing:
Redirect stdout to text file, open this file using LibreOffice Writer.

Use page format A4, 10mm margins for top and bottom, 20mm margins (default) for left and right.

As font, use Fira Code 14.5 pt.

Now there should be two sudokus per page, ready for printing.

Be careful if you want to print to pdf with 2 pages per sheet, (at least on my machine) LibreOffice Writer
assumes you are printing on US Letter format and it refuses to print 2 pages on one A4 sheet.