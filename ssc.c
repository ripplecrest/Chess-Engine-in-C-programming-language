#include<stdio.h>

//bitboard datatype
#define U64 unsigned long long

// set/get/pop macros
#define get_bit(bitboard, square) (bitboard &(1ULL << square)) 
#define set_bit(bitboard, square) (bitboard |= (1ULL << square)) // add 1 to the position in the bitboard
#define pop_bit(bitboard, square) (set_bit(bitboard, square)? bitboard ^= (1ULL << square): 0) // makes the bit at the position 0 again.

//funtion to count bit within bitboard
static inline int count_bits(U64 bitboard){
    // bit count
    int count = 0;
    //consecutively reset least significant 1st bit
    while(bitboard){
        count++;
        // reset least significant 1st bit
        bitboard &= bitboard -1;
    }
    return count;
}
// get least significant bit index
static inline int get_ls1b_index(U64 bitboard){
    // makesure bitboard != 0
    if(bitboard){
        // count trailing bits before LS1B
        return count_bits((bitboard& -bitboard)-1);
    }
    else{
        // return illegal index
        return -1;
    }
}

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

const char *square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8",  "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7",  "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6",  "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5",  "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4",  "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3",  "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2",  "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1",  "g1", "h1"
};
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

// bishop relevant occupancy bit count for every square on board.
const int bishop_relevant_bits[64]= {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

// rook relevant occupancy bit count for every square on board.
const int rook_relevant_bits[64]= {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12
};

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
    // init ranks and files
    int r,f;
    // init target rank and files
    int tr = square/8;
    int tf = square % 8;
    
    // mask relevent bishop occupancy squares
    for(r = tr+1, f = tf+1; r<=6 && f<=6; r++, f++) attacks |= (1ULL << (r*8 + f));
    for(r = tr-1, f = tf+1; r>=1 && f<=6; r--, f++) attacks |= (1ULL << (r*8 + f));

    for(r = tr+1, f = tf-1; r<=6 && f>=1; r++, f--) attacks |= (1ULL << (r*8 + f));
    for(r = tr-1, f = tf-1; r>=1 && f>=1; r--, f--) attacks |= (1ULL << (r*8 + f));
    return attacks;
}
// mask rook attack
U64 mask_rook_attacks(int square){
     // result attack bitboard
    U64 attacks = 0ULL;
    // init ranks and files
    int r,f;
    // init target rank and files
    int tr = square/8;
    int tf = square % 8;
    
    // mask relevent rook occupancy squares
    for(r = tr+1; r<=6; r++) attacks |= (1ULL << (r*8 +tf));
    for(r = tr-1; r>=1; r--) attacks |= (1ULL << (r*8 +tf));
    for(f = tf+1; f<=6; f++) attacks |= (1ULL << (tr*8 +f));
    for(f = tf-1; f>=1; f--) attacks |= (1ULL << (tr*8 +f));

    return attacks;
}
U64 bishop_attacks_on_the_fly(int square, U64 block){
     // result attack bitboard
    U64 attacks = 0ULL;
    // init ranks and files
    int r,f;
    // init target rank and files
    int tr = square/8;
    int tf = square % 8;
    
    // generate bishop attack

    for(r = tr+1, f = tf+1; r<=7 && f<=7; r++, f++) {
        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8+f)) & block) break;
    }
    for(r = tr-1, f = tf+1; r>=0 && f<=7; r--, f++) {
        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8+f)) & block) break;
    }

    for(r = tr+1, f = tf-1; r<=7 && f>=0; r++, f--) {
        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8+f)) & block) break;
    }
    for(r = tr-1, f = tf-1; r>=0 && f>=0; r--, f--) {
        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8+f)) & block) break;
    }
    return attacks;
}
U64 rook_attacks_on_the_fly(int square, U64 block){
     // result attack bitboard
    U64 attacks = 0ULL;
    // init ranks and files
    int r,f;
    // init target rank and files
    int tr = square/8;
    int tf = square % 8;
    
    // generate rook attack
    for(r = tr+1; r<=7; r++) {
        attacks |= (1ULL << (r*8 +tf));
        if((1ULL << (r*8+tf))& block) break;
    }
    for(r = tr-1; r>=0; r--) {
        attacks |= (1ULL << (r*8 +tf));
        if((1ULL << (r*8+tf))& block) break;
    }
    for(f = tf+1; f<=7; f++) {
        attacks |= (1ULL << (tr*8 +f));
        if((1ULL << (tr*8 +f))& block) break;

    }
    for(f = tf-1; f>=0; f--) {
        attacks |= (1ULL << (tr*8 +f));
        if((1ULL << (tr*8 +f))& block) break;
    }

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
// set occupancies
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask){
    // init occupancy map
    U64 occupancy = 0ULL;
    // loop over range of bits withi attack mask
    for(int count = 0; count < bits_in_mask; count++){
        // get LS1B index of attack mask
        int square = get_ls1b_index(attack_mask);
        // pop LS1B in attack map
        pop_bit(attack_mask, square);

        // make sure that occupancy is on the board
        if(index & (1 << count)){
            //poppulate occupancy map
            occupancy |= (1ULL << square);
        }
    }

    return occupancy;
}

// pseudo random number state
unsigned int state = 1804289383;

// generate 32bit pseudo legal numbers 
unsigned int get_random_number(){
    // get current state
    unsigned int number = state;
    // XOR shift algorithm (in 32 bits)
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    state = number; // updaet random number state
    return number;
}


int main(){
    //init leaper piece attacks
    init_leapers_attacks();
    printf("%ud\n", get_random_number());
    printf("%ud\n", get_random_number());
    printf("%ud\n", get_random_number());
    printf("%ud\n", get_random_number());
    printf("%ud\n", get_random_number());
    return 0;
}