#include <iostream>
#include <array>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>

#include "args_parser.hpp";

typedef uint8_t byte;

const std::array<byte, 256> sbox = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const std::array<byte, 256> inv_sbox = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

const std::array<byte, 10> Rcon = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

const int BLOCK_SIZE = 16;
const int EXPANDED_BLOCK_SIZE = 176;

std::array<byte, BLOCK_SIZE> state;

byte galois_multiplication(byte a, byte b)
{
    byte result = 0;
    while (b)
    {
        if (b & 0x01) result ^= a;
        byte highBit = a & 0x80;
        a <<= 1;
        if (highBit) a ^= 0x1b;
        b >>= 1;
    }
    return result;
}

std::array<byte, 4> rot_word(const std::array<byte, 4>& word) 
{
    return { word[1], word[2], word[3], word[0] };
}

std::array<byte, 4> sub_word(const std::array<byte, 4>& word) 
{
    return { sbox[word[0]], sbox[word[1]], sbox[word[2]], sbox[word[3]] };
}

std::array<byte, EXPANDED_BLOCK_SIZE> key_expansion(const std::array<byte, BLOCK_SIZE>& key) 
{
    std::array<byte, EXPANDED_BLOCK_SIZE> expanded_key = {};
    std::copy(key.begin(), key.end(), expanded_key.begin());

    int bytes_generated = 16;
    int rcon_index = 0;

    std::array<byte, 4> temp;

    while (bytes_generated < EXPANDED_BLOCK_SIZE) 
    {
        // last 4 bytes from the currently expanded key
        temp = { expanded_key[bytes_generated - 4], expanded_key[bytes_generated - 3],
                 expanded_key[bytes_generated - 2], expanded_key[bytes_generated - 1] };

        if (bytes_generated % 16 == 0) 
        {
            temp = sub_word(rot_word(temp));
            temp[0] ^= Rcon[rcon_index++];
        }

        for (int i = 0; i < 4; ++i) 
        {
            expanded_key[bytes_generated] = expanded_key[bytes_generated - 16] ^ temp[i];
            ++bytes_generated;
        }
    }

    return expanded_key;
}

void sub_bytes()
{
    for (byte& b : state)
    {
        b = sbox[b];
    }
}

void inv_sub_bytes() 
{
    for (byte& b : state) 
    {
        b = inv_sbox[b];
    }
}

void shift_rows()
{
    std::rotate(state.begin() + 4, state.begin() + 5, state.begin() + 8);
    std::rotate(state.begin() + 8, state.begin() + 10, state.begin() + 12);
    std::rotate(state.begin() + 12, state.begin() + 15, state.begin() + 16);
}

void inv_shift_rows() 
{
    std::rotate(state.begin() + 4, state.begin() + 7, state.begin() + 8);
    std::rotate(state.begin() + 8, state.begin() + 10, state.begin() + 12);
    std::rotate(state.begin() + 12, state.begin() + 13, state.begin() + 16);
}

void mix_columns()
{
    for (int col = 0; col < 4; ++col) 
    {
        byte s0 = state[col];
        byte s1 = state[4 + col];
        byte s2 = state[8 + col];
        byte s3 = state[12 + col];

        state[col] = galois_multiplication(s0, 0x02) ^ galois_multiplication(s1, 0x03) ^ s2 ^ s3;
        state[4 + col] = s0 ^ galois_multiplication(s1, 0x02) ^ galois_multiplication(s2, 0x03) ^ s3;
        state[8 + col] = s0 ^ s1 ^ galois_multiplication(s2, 0x02) ^ galois_multiplication(s3, 0x03);
        state[12 + col] = galois_multiplication(s0, 0x03) ^ s1 ^ s2 ^ galois_multiplication(s3, 0x02);
    }
}

void inv_mix_columns() 
{
    for (int col = 0; col < 4; ++col) 
    {
        byte s0 = state[col];
        byte s1 = state[4 + col];
        byte s2 = state[8 + col];
        byte s3 = state[12 + col];

        state[col] = galois_multiplication(s0, 0x0e) ^ galois_multiplication(s1, 0x0b) ^ galois_multiplication(s2, 0x0d) ^ galois_multiplication(s3, 0x09);
        state[4 + col] = galois_multiplication(s0, 0x09) ^ galois_multiplication(s1, 0x0e) ^ galois_multiplication(s2, 0x0b) ^ galois_multiplication(s3, 0x0d);
        state[8 + col] = galois_multiplication(s0, 0x0d) ^ galois_multiplication(s1, 0x09) ^ galois_multiplication(s2, 0x0e) ^ galois_multiplication(s3, 0x0b);
        state[12 + col] = galois_multiplication(s0, 0x0b) ^ galois_multiplication(s1, 0x0d) ^ galois_multiplication(s2, 0x09) ^ galois_multiplication(s3, 0x0e);
    }
}

void add_round_key(int round, const std::array<byte, EXPANDED_BLOCK_SIZE>& expanded_key)
{
    int start = round * BLOCK_SIZE;
    for (size_t i = 0; i < BLOCK_SIZE; ++i)
    {
        state[i] ^= expanded_key[start + i];
    }
}

void encrypt(const std::array<byte, BLOCK_SIZE>& key)
{
    auto expanded_key = key_expansion(key);
    add_round_key(0, expanded_key);

    for (int round = 1; round < 10; ++round) 
    {
        sub_bytes();
        shift_rows();
        mix_columns();
        add_round_key(round, expanded_key);
    }

    // final round without mix_columns
    sub_bytes();
    shift_rows();
    add_round_key(10, expanded_key);
}

void decrypt(const std::array<byte, BLOCK_SIZE>& key)
{
    auto expanded_key = key_expansion(key);
    add_round_key(10, expanded_key);

    for (int round = 9; round >= 1; --round) 
    {
        inv_shift_rows();
        inv_sub_bytes();
        add_round_key(round, expanded_key);
        inv_mix_columns();
    }

    inv_shift_rows();
    inv_sub_bytes();
    add_round_key(0, expanded_key);
}

std::vector<byte> pad_block(const std::vector<byte>& data) 
{
    std::vector<byte> padded = data;
    size_t padding_length = 16 - (data.size() % 16);
    padded.insert(padded.end(), padding_length, static_cast<byte>(padding_length));
    return padded;
}

std::vector<byte> aes_encrypt_file(const std::vector<byte>& input_data, const std::array<byte, BLOCK_SIZE>& key) 
{
    std::vector<byte> encrypted_data;

    // TODO decide on padding per block and not beforehand, otherwise the entire file in memory has to be copied
    std::vector<byte> padded_data = input_data.size() % 16 == 0 ? input_data : pad_block(input_data);

    for (size_t i = 0; i < padded_data.size(); i += 16) 
    {
        std::array<byte, 16> block;
        std::copy_n(padded_data.begin() + i, 16, block.begin());

        state = block;
        encrypt(key);
        block = state;

        encrypted_data.insert(encrypted_data.end(), block.begin(), block.end());
    }

    return encrypted_data;
}

std::vector<byte> aes_decrypt_file(const std::vector<byte>& input_data, const std::array<byte, BLOCK_SIZE>& key)
{
    std::vector<byte> decrypted_data;

    for (size_t i = 0; i < input_data.size(); i += 16)
    {
        std::array<byte, BLOCK_SIZE> block;
        std::copy_n(input_data.begin() + i, BLOCK_SIZE, block.begin());

        state = block;
        decrypt(key);
        block = state;

        decrypted_data.insert(decrypted_data.end(), block.begin(), block.end());
    }

    // assuming PKCS#7 padding
    if (!decrypted_data.empty()) 
    {
        byte padding_length = decrypted_data.back();
        if (padding_length > 0 && padding_length <= BLOCK_SIZE) 
        {
            decrypted_data.resize(decrypted_data.size() - padding_length);
        }
    }

    return decrypted_data;
}

int main(int argc, char* argv[])
{
    const Arguments args = parse_args(argc, argv);
    if (args.error)
    {
        std::cerr << "Usage: " << argv[0] << " -e|-d <mode> <input_file> <passphrase>\n" << std::endl;
        return EXIT_FAILURE;
    }

    // TODO add key derivation function, for now a hardcoded key is enough
    std::array<byte, BLOCK_SIZE> key = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };

    std::ifstream input_file(args.input_filename, std::ios::binary);
    if (!input_file.is_open())
    {
        return EXIT_FAILURE;
    }
    // TODO read and encrypt data in chunks and don't load the entire file into memory
    std::vector<byte> input_data((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

    // TODO use something other than ECB mode by default
    // TODO rework block iteration to directly write a certain amount of blocks to the output, otherwise the entire ciphertext has to be put into memory
    std::vector<byte> data = args.operation == Encryption ? aes_encrypt_file(input_data, key) : aes_decrypt_file(input_data, key);

    std::ofstream output_file(args.output_filename, std::ios::binary | std::ios::trunc);
    if (!output_file.is_open())
    {
        return EXIT_FAILURE;
    }
    output_file.write(reinterpret_cast<const char*>(data.data()), data.size());

    return EXIT_SUCCESS;
}
