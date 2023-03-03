#ifndef TPARSER_H
#define TPARSER_H

#include "scew/scew.h"

#ifdef __cplusplus
extern "C"
{
#endif

void tree_set_encoding(scew_tree* pstTree, const char* pszEncoding);
void tree_set_default_encoding(scew_tree* pstTree);
void tree_set_standalone(scew_tree* pstTree, int standalone);

unsigned int tree_save_file(scew_tree* pstTree, const char* pszFile);

void tparser_convert_string(unsigned char* pszStr);
void tparser_convert_element(scew_element* pstElement);
int tparser_set_encoding(scew_parser* pstParser, const char* pszEncoding);
int tparser_set_default_encoding(scew_parser* pstParser);

scew_parser* tparser_create(void);
#define tparser_free		scew_parser_free

#define tparser_load_file	scew_parser_load_file
#define tparser_load_fp		scew_parser_load_file_fp
#define tparser_load_buffer	scew_parser_load_buffer

scew_tree* tparser_tree(scew_parser* pstParser);



#ifdef __cplusplus
}
#endif

#endif /* TPARSER_H */
