#include "UA2_Text_Box.h"#include	"Diffract_INCs.h"void			DA2_Text_Box::SetRowTitle(char* titleText,short row){	long length;	length = strlen(titleText);	TMX_SetRowTitle((Ptr)titleText,length,row,theList);}void			DA2_Text_Box::SetColumnTitle(char* titleText,short row){	long length;	length = strlen(titleText);	TMX_SetColumnTitle((Ptr)titleText,length,row,theList);}