//a_FileObject.h#ifndef	_FILE_OBJ#define	_FILE_OBJclass	FileObj{public:	short					pathRefNum;	long					xferLength;	Boolean					file_is_Open;	StandardFileReply		theFileInfo;	OSErr					fileError;	void					ReadFileOpen(OSType	fileType);			/* open the File			*/	void					SaveFileOpen(OSType	fileType);	void					FileOpen(void);			Boolean					ReadBoolean(void);	void					WriteBoolean(Boolean theData);	/* Jim's Code */	void					WriteShortDouble(short double theData);		short double			ReadShortDouble(void);	void					WriteStructureBlock(void*	theData,long	ptrSize);	void					ReadStructureBlock(void*	theData,long	dataLength);	/* Jim's Code End */	short					ReadShort		(void);	void					WriteShort		(short theData);	long					ReadLong		(void);	void					WriteLong		(long theData);	float					ReadFloat		(void);	void					WriteFloat		(float theData);			void					WritePtr		(void* theData);	void*					ReadPtr			(void* theData);	void					WriteHandle		(Handle theData);	void					ReadHandle		(Handle theData);	void					WriteDataBlock	(void*	theData,long	dataLength);	void					ReadDataBlock	(void*	theData,long	dataLength);	void					DoFileClose		(void);	void					DoFileError		(void);	void					DoFileAlert		(void);	void					WritePICT		(Ptr theObj);	PicHandle				ReadPICT		(void);	void					WriteTextHandle	(Handle	theData);	void					WriteTextPtr	(Ptr	theData);	Boolean  			CreatePref(void);	void 					WriteResType(ResType theData);	ConstStr255Param ReadConstStr255(void);	void 					WriteConstStr255(ConstStr255Param theData);	ResType 			ReadResType(void);	void 					WriteAxis(Axis vector);	void 					ReadAxis(Axis *vector);	void 					ReadVector(Vector *vector);	void 					WriteVector(Vector vector);	void 					ReadMatrix(double matrix[3][3]);	void 					WriteMatrix(double matrix[3][3]);	void 					WriteRect(Rect theRect);	void 					ReadRect(Rect* theRect);	Boolean  			CreateNewFile(OSType	fileType,char *theText);}; pascal void SavePICTData(Ptr theData, short length);#endif