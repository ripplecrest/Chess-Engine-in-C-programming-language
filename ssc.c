#include<stdio.h>

//bitboard datatype
#define U64 unsigned long long

// set/get/pop macros
#define get_bit(bitboard, square) (bitboard &(1ULL << square)) 
#define set_bit(bitboard, square) (bitboard |= (1ULL << square)) // add 1 to the position in the bitboard
#define pop_bit(bitboard, square) (set_bit(bitboard, square)? bitboard ^= (1ULL << square): 0;) // makes the bit at the position 0 again.


//board squares
enum{
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1

};
enum{
    white, black
};

/*
"a8", "b8", "c8", "d8", "e8", "f8",  "g8", "h8"
"a7", "b7", "c7", "d7", "e7", "f7",  "g7", "h7"
"a6", "b6", "c6", "d6", "e6", "f6",  "g6", "h6"
"a5", "b5", "c5", "d5", "e5", "f5",  "g5", "h5"
"a4", "b4", "c4", "d4", "e4", "f4",  "g4", "h4"
"a3", "b3", "c3", "d3", "e3", "f3",  "g3", "h3"
"a2", "b2", "c2", "d2", "e2", "f2",  "g2", "h2"
"a1", "b1", "c1", "d1", "e1", "f1",  "g1", "h1"
*/

//print bitboard

void print_bitboard(U64 bitboard){
    // loop over ranks(rows)
    for(int rank = 0; rank < 8; rank++){

        //loop over files(colums)
        for(int file = 0; file < 8; file++){
            // conver file and rank into square index.
            int square = rank*8 + file;

            // print rank
            if(!file){
                printf(" %d ", 8-rank);
            }
            // print the bit state (either 0 or 1)
            printf(" %d", get_bit(bitboard, square)?1:0);
        }
        printf("\n");
    }
    // print file
    printf("\n    a b c d e f g h \n\n");
    // print bitboard as unsigned decimal number
    printf("    Bitboard: = %llud\n\n", bitboard);
}

// attacks
/*
    Not A file
    8  0 1 1 1 1 1 1 1
    7  0 1 1 1 1 1 1 1
    6  0 1 1 1 1 1 1 1
    5  0 1 1 1 1 1 1 1
    4  0 1 1 1 1 1 1 1
    3  0 1 1 1 1 1 1 1
    2  0 1 1 1 1 1 1 1
    1  0 1 1 1 1 1 1 1

       a b c d e f g h 
    
    Not H file
    8  1 1 1 1 1 1 1 0
    7  1 1 1 1 1 1 1 0
    6  1 1 1 1 1 1 1 0
    5  1 1 1 1 1 1 1 0
    4  1 1 1 1 1 1 1 0
    3  1 1 1 1 1 1 1 0
    2  1 1 1 1 1 1 1 0
    1  1 1 1 1 1 1 1 0

       a b c d e f g h 

    Not HG files
    8  1 1 1 1 1 1 0 0
    7  1 1 1 1 1 1 0 0
    6  1 1 1 1 1 1 0 0
    5  1 1 1 1 1 1 0 0
    4  1 1 1 1 1 1 0 0
    3  1 1 1 1 1 1 0 0
    2  1 1 1 1 1 1 0 0
    1  1 1 1 1 1 1 0 0

       a b c d e f g h 

    Not AB file
    8  0 0 1 1 1 1 1 1
    7  0 0 1 1 1 1 1 1
    6  0 0 1 1 1 1 1 1
    5  0 0 1 1 1 1 1 1
    4  0 0 1 1 1 1 1 1
    3  0 0 1 1 1 1 1 1
    2  0 0 1 1 1 1 1 1
    1  0 0 1 1 1 1 1 1

        a b c d e f g h 

*/
// Not A file constant

const U64 not_a_file = 18374403900871474942ULL;
// not H file constant
const U64 not_h_file = 9187201950435737471ULL;
// not HG file
const U64 not_hg_file = 4557430888798830399ULL;
// not AB file
const U64 not_ab_file = 18229723555195321596ULL;

// Pawn attack
U64 pawn_attacks[2][64];
// knight attack table
U64 knight_attacks[64]; //does not depend on the colour.
//king attack table
U64 king_attacks[64];

//generate pawn attack 
U64 mask_pawn_attack(int side, int square){
    // result attack bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set piece on the bitboard
    set_bit(bitboard, square);
    // for white
    if(!side){
        if((bitboard >> 7)& not_a_file){ // if the pawn is on the h file it will be zero hence no attack on the a file.
            attacks |= (bitboard >> 7);
        } 
        if((bitboard >> 9)& not_h_file){ // id the pawn is on the a file it not not attack the h file.
            attacks |= (bitboard >> 9);
        } 
        // The above two conditions are very important in pawn capture case. 
    }
    // for black
    else{
         if((bitboard << 7)& not_h_file){ // if the pawn is on the h file it will be zero hence no attack on the a file.
            attacks |= (bitboard << 7);
        } 
        if((bitboard << 9)& not_a_file){ // id the pawn is on the a file it not not attack the h file.
            attacks |= (bitboard << 9);
        } 

    }
    return attacks;
}
// generate knight attack
U64 mask_knight_attack(int square){
    // result attack bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set piece on the bitboard
    set_bit(bitboard, square);
    // generate knight attacks 17, 15, 10 ,6
    if((bitboard>>17)& not_h_file)attacks |= (bitboard >> 17);
    if((bitboard>>15)& not_a_file)attacks |= (bitboard >> 15);
    if((bitboard>>10)& not_hg_file) attacks |= (bitboard >> 10);
    if((bitboard>>6)& not_ab_file) attacks |= (bitboard >> 6);

    // opposite side attacks
    if((bitboard<<17)& not_a_file)attacks |= (bitboard << 17);
    if((bitboard<<15)& not_h_file)attacks |= (bitboard << 15);
    if((bitboard<<10)& not_ab_file) attacks |= (bitboard << 10);
    if((bitboard<<6)& not_hg_file) attacks |= (bitboard << 6);
    return attacks;

}
//generates king attacks
U64 mask_king_attack(int square){
    // result attack bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set piece on the bitboard
    set_bit(bitboard, square);
    // generate king attacks
    if (bitboard >> 8 )attacks |= (bitboard >> 8);
    if ((bitboard >> 9) & not_h_file )attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file )attacks |= (bitboard >> 7);
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);

    // backwards attack
    if (bitboard << 8 )attacks |= (bitboard << 8);
    if ((bitboard << 9) & not_a_file )attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file )attacks |= (bitboard << 7);
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);
    return attacks;

}
// mask bishop attacks
U64 mask_bishop_attacks(int square){
     // result attack bitboard
    U64 attacks = 0ULL;

    return attacks;
}

//init leaper pirces attacks
void init_leapers_attacks(){
    // loop over 64 boards squares
    for (int square = 0; square < 64; square++){
        // init pawn attacks 
        pawn_attacks[white][square] = mask_pawn_attack(white, square);
        pawn_attacks[black][square] = mask_pawn_attack(black, square);

        //init knight attack
        knight_attacks[square] = mask_knight_attack(square);

        // init king attacks
        king_attacks[square] = mask_king_attack(square);
    }
}

int main(){
    //init leaper piece attacks
    init_leapers_attacks();
    // loop over 64 board squares
      for (int square = 0; square < 64; square++)
         print_bitboard(king_attacks[square]);
    //print_bitboard(mask_knight_attack(e3));
  
    //print_bitboard(mask_king_attack(h4));
    
    return 0;
}