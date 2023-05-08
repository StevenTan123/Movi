#ifndef __MOVE_STRUCTURE__
#define __MOVE_STRUCTURE__

#include <fstream>
#include <cstdint>
#include <zlib.h>
#include <stdio.h>
#include <chrono>
#include <cstddef>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include <unordered_map>

#include "kseq.h"
#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>

#include "move_row.hpp"
#include "move_query.hpp"

#define END_CHARACTER 0
#define THRBYTES 5 

class MoveStructure {
    public:
        MoveStructure() { }
        MoveStructure(bool verbose_ = false, bool logs_ = false);
        MoveStructure(char* input_file, bool bit1_ = false, bool verbose_ = false);

        void build(std::ifstream &bwt_file);
        uint64_t query_ms(MoveQuery& mq, bool random);
        void all_lf_test(/*std::ifstream &bwt_file*/);
        uint64_t random_lf_test();
        std::string reconstruct();
        std::string reconstruct_move();

        uint64_t LF(uint64_t row_number);
        uint64_t LF_move(uint64_t& pointer, uint64_t& i);
        uint64_t fast_forward(uint64_t pointer, uint64_t index);
        uint64_t fast_forward(uint16_t& offset, uint64_t index, uint64_t x);
        char compute_char(uint64_t idx);
        uint64_t compute_threshold(uint64_t r_idx, uint64_t pointer, char lookup_char);
        uint32_t compute_index(char row_char, char lookup_char);

        uint64_t naive_lcp(uint64_t row1, uint64_t row2);
        uint64_t naive_sa(uint64_t bwt_row);

        uint64_t jump_up(uint64_t idx, char c);
        uint64_t jump_down(uint64_t idx, char c);
        bool jump_thresholds(uint64_t& idx, uint64_t pointer, uint16_t offset, char r_char);
        bool jump_randomly(uint64_t& idx, char r_char);
        bool jump_naive_lcp(uint64_t& idx, uint64_t pointer, char r_char, uint64_t& lcp);
        void compute_nexts();

        void serialize(char* output_dir);
        void deserialize(char* index_dir);
        
        std::unordered_map<uint32_t, uint32_t> jumps;
        std::unordered_map<uint32_t, uint32_t> ff_counts;

    private:
        bool bit1;
        std::string bwt_string;
        std::string orig_string;
        bool reconstructed;
        uint64_t length;
        uint64_t r;
        uint64_t end_bwt_row;
        uint64_t end_bwt_row_thresholds[4];
        bool verbose;
        bool logs;

        std::vector<unsigned char> alphabet;
        std::vector<uint64_t> counts;
        std::vector<uint64_t> alphamap;

        std::vector<std::unique_ptr<sdsl::bit_vector> > occs;
        std::vector<std::unique_ptr<sdsl::rank_support_v<> > > occs_rank;
        sdsl::bit_vector bits;
        sdsl::rank_support_v<> rbits;
        sdsl::select_support_mcl<> sbits;
        sdsl::int_vector<> thresholds;

        std::vector<MoveRow> rlbwt;
        // std::vector<char> rlbwt_chars;
        uint64_t eof_row;
        uint64_t bit1_begin;
        uint64_t bit1_after_eof;
};

#endif
