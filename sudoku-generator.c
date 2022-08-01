#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

char* new_sudoku()
{
    char* result = (char*) malloc(9*9 * sizeof(char));
    for(int i = 0; i < 81; i++)
    {
        result[i] = ' ';
    }
    return result;
}


char* transpose(char matrix[81])
{
    char* result = (char*) malloc(9*9 * sizeof(char));
    for(int col = 0; col < 9; col++)
    {
        for(int row = 0; row < 9; row++)
        {
            result[row*9 + col] = matrix[col*9 + row];
        }
    }
    return result;
}


char* copy_sudoku(char matrix[81])
{
    char* result = (char*) malloc(9*9 * sizeof(char));
    for(int i = 0; i < 81; i++)
    {
        result[i] = matrix[i];
    }
    return result;
}


bool sudoku_equal(char s1[81], char s2[81])
{
    for(int i = 0; i < 81; i++)
    {
        if(s1[i] != s2[i])
        {
            return false;
        }
    }
    return true;
}


//the chars in list have to be numerical characters from '1' to '9'
bool duplicates(char list[9])
{
    bool checklist[9] = {false,false,false,false,false,false,false,false,false};
    for(int i = 0; i < 9; i++)
    {
        if(list[i] != ' ')
        {
            if(checklist[list[i] - '1']) //if this value has been seen before
            {
                return true;
            }
            checklist[list[i] - '1'] = true;
        }
    }
    return false;
}


bool no_rules_broken_at_index(char matrix[81], int index)
{
    int row = index / 9;
    int col = index % 9;
    if(duplicates(matrix + (row*9)))
    {
        return false;
    }
    char* matrix_transposed = transpose(matrix);
    if(duplicates(matrix_transposed + (col*9)))
    {
        free(matrix_transposed);
        return false;
    }
    free(matrix_transposed);
    int block_row = index / 27;
    int block_col = (index/3) % 3;
    char block_content[9];
    int i = 0;
    for(int row_ = block_row * 3; row_ < (block_row+1) * 3; row_++)
    {
        for(int col_ = block_col * 3; col_ < (block_col+1) * 3; col_++)
        {
            block_content[i] = matrix[row_*9 + col_];
            i++;
        }
    }
    if(duplicates(block_content))
    {
        return false;
    }
    return true;
}



void print_sudoku(char matrix[81], int printnumber, void* destination)
{
    fprintf(destination, "%d\n", printnumber);
    fprintf(destination, "         ┏━━━┯━━━┯━━━┳━━━┯━━━┯━━━┳━━━┯━━━┯━━━┓\n");
    for(int row = 0; row < 9; row++)
    {
        fprintf(destination, "         ┃");
        for(int col = 0; col < 9; col++)
        {
            fprintf(destination, " %c ", matrix[row*9 + col]);
            if(col == 2 || col == 5 || col == 8)
            {
                fprintf(destination, "┃");
            }
            else
            {
                fprintf(destination, "│");
            }
        }
        if(row == 2 || row == 5)
        {
            fprintf(destination, "\n         ┣━━━┿━━━┿━━━╋━━━┿━━━┿━━━╋━━━┿━━━┿━━━┫\n");
        }
        else if (row < 8)
        {
            fprintf(destination, "\n         ┠───┼───┼───╂───┼───┼───╂───┼───┼───┨\n");
        }
    }
    fprintf(destination, "\n         ┗━━━┷━━━┷━━━┻━━━┷━━━┷━━━┻━━━┷━━━┷━━━┛\n");
}



char* rand_char_permutation()
{
    char* result = (char*) malloc(9 * sizeof(char));
    for(int i = 0; i < 9; i++)
    {
        result[i] = ' ';
    }
    for(char c = '1'; c <= '9'; c++)
    {
        int position;
        for (position = rand() % 9; result[position] != ' '; position = (position + 1) % 9) {} //walk through result beginning at random index until empty spot is found
        result[position] = c;
    }
    return result;
}



int* rand_index_permutation()
{
    int* result = (int*) malloc(81 * sizeof(int));
    for(int i = 0; i < 81; i++)
    {
        result[i] = -1;
    }
    for(int index = 0; index < 81; index++)
    {
        int position;
        for (position = rand() % 81; result[position] != -1; position = (position + 1) % 81) {} //walk through result beginning at random index until empty spot is found
        result[position] = index;
    }
    return result;
}



//in situ
//order determines the order in which the numbers are inserted. 0 = low numbers first, 1 = high first, 2 = random order
bool sudoku_insert(char matrix[81], int index, int order)
{
    if(index >= 81)
    {
        return true;
    }
    if(matrix[index] != ' ')
    {
        return sudoku_insert(matrix, index + 1, order);
    }
    char* insert_permutation = malloc(10*sizeof(char));
    switch(order)
    {
        case 0:
            strcpy(insert_permutation, "123456789");
            break;
        case 1:
            strcpy(insert_permutation, "987654321");
            break;
        case 2:
            free(insert_permutation);
            insert_permutation = rand_char_permutation();
    }
    for(int i = 0; i < 9; i++) //try inserting
    {
        matrix[index] = insert_permutation[i];
        if(no_rules_broken_at_index(matrix, index))
        {
            if(sudoku_insert(matrix, index+1, order))
            {
                free(insert_permutation);
                return true;
            }
        }
    }
    //no insertion was successful
    free(insert_permutation);
    matrix[index] = ' ';
    return false;    
}



int number_of_clues(char* matrix)
{
    int result = 0;
    for(int i = 0; i < 81; i++)
    {
        result += (int) (matrix[i] != ' ');
    }
    return result;
}



bool sudoku_is_unambiguous(char* s)
{
    char* s0 = copy_sudoku(s);
    char* s1 = copy_sudoku(s);
    sudoku_insert(s0, 0, 0);
    sudoku_insert(s1, 0, 1);
    bool result = sudoku_equal(s0, s1);
    free(s0);
    free(s1);
    return result;
}



//in situ
void sudoku_delete_numbers(char matrix[81], int n)
{
    int* index_permutation = rand_index_permutation();
    for(int i = 0; i < 81; i++)
    {
        if(matrix[index_permutation[i]] != ' ')
        {
            char removed_value = matrix[index_permutation[i]];
            matrix[index_permutation[i]] = ' ';
            if(sudoku_is_unambiguous(matrix))
            {
                if(--n == 0)
                {
                    return;
                }
            }
            else
            {
                matrix[index_permutation[i]] = removed_value; //put it back
            }
        }
    }
    //if this is reached, there is no number that can be deleted, the current sudoku in matrix is a minimal sudoku
}



void generate_unambiguous_sudoku(int n, int printnumber)
{
    char* sudoku = new_sudoku();
    sudoku_insert(sudoku, 0, 2);
    char* solution = copy_sudoku(sudoku);
    sudoku_delete_numbers(sudoku, 81-n);
    print_sudoku(sudoku, printnumber, stdout);
    print_sudoku(solution, printnumber, stderr);
    //printf("number of clues: %d\n", number_of_clues(sudoku));
    free(sudoku);
    free(solution);
}



int main(int argc, char* argv[])
{
    int no_of_sudokus = 1;
    int label_offset = 1;
    if(argc >= 1)
    {
        no_of_sudokus = atoi(argv[1]);
    }
    if(argc >= 2)
    {
        label_offset = atoi(argv[2]);
    }

    srand(time(NULL));

    for(int i = 0; i < no_of_sudokus; i++)
    {
        generate_unambiguous_sudoku(17, i + label_offset);

        if(i % 2 == 0)
        {
            fprintf(stdout, "\n\n\n");
        }
    }
}