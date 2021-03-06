struct _tag {
	dword start;
	dword name;
	dword param[10];
	dword value[10];
	void parce();
	int nameis();
	void clear();
};

void _tag::parce()
{
	dword o = name = start;
	while (ESBYTE[o]!=' ') && (ESBYTE[o]) o++; //searching for a space after tag name
	ESBYTE[o] = '\0';
	strlwr(name);
}

int _tag::nameis(dword _in_tag_name)
{
	if (name) && (strcmp(_in_tag_name, name)==0) return true;
	return false;
}

void _tag::clear() 
{
	start=name=0;
}


/*
unsigned int GetNextParam()
{
	byte	kavichki=0;
	int		i = strlen(#tagparam) - 1;
	
	if (!tagparam) return 0;
	
	WHILE((i > 0) && ((tagparam[i] == '"') || (tagparam[i] == ' ') || (tagparam[i] == '\'') || (tagparam[i] == '/')))
	{
		IF (tagparam[i] == '"') || (tagparam[i] == '\'') kavichki=tagparam[i];
		tagparam[i] = 0x00;
		i--;
	}

	if (kavichki)
	{
		i=strrchr(#tagparam, kavichki);
		strlcpy(#val, #tagparam + i, sizeof(val));
	}
	else
	{
		WHILE((i > 0) && (tagparam[i] <>'=')) i--; //i=strrchr(#tagparam, '=')+1;
		i++;
		strlcpy(#val, #tagparam + i, sizeof(val));

		WHILE (val[0] == ' ') strcpy(#val, #val+1);
	}
	tagparam[i] = 0x00;

	FOR ( ; ((tagparam[i] <>' ') && (i > 0); i--)
	{
		IF (tagparam[i] == '=') //äåðçêàÿ çàãëóøêà
			tagparam[i + 1] = 0x00;
	}
	strlcpy(#attr, #tagparam + i + 1, sizeof(attr));
	tagparam[i] = 0x00;
	strlwr(#attr);
	return 1;
}
*/

//
//   STYLE
//

struct _style {
	bool b, u, i, s;
	bool h1, h2, h3, h4, h5, h6;
	bool a;
	bool pre;
	bool ignore;
	dword color;
	void clear();
} style;

void _style::clear()
{
	b=u=i=s=0;
	h1=h2=h3=h4=h5=h6=0;
	a=0;
	pre=0;
	ignore=0;
	color=0;
}


//
//   TEXT
//

struct _text {
	dword start;
	char str[7];
	int x, y;
	void fixSpecial(dword text);
};


// unicode conversion for special characters
char *unicode_tags[]={
"nbsp",  " ",
"#38",   " ",
"#160",  " ",

"ntilde", "n", // spanish n special ñ
"#224",   "à", // spanish a with grove accent 'à'
"#241",  "n", // spanish symbol

"copy",  "(c)",
"#169",  "(c)",

"trade", "[TM]",

"reg",   "(r)",
"#174",  "(r)",

"bdquo", ",,",

"amp",   "&",
"#38",   "&",

"lt",    "<",
"#60",   "<",

"gt",    ">",
"#62",   ">",

"minus", "-",
"ndash", "-",
"mdash", "-", //--
"#8722", "-",
"#8211", "-",
"#151",  "-",
"#149",  "-",

"rsquo", "'",
"#39",   "'",
"#96",   "'",
"#8217", "'",

"quot",  "\"",
"#34",   "\"",
"ldquo", "\"",
"rdquo", "\"",
"#8222", "\"",
"#8221", "\"",

"laquo", "<<",
"#171",  "<<",
"raquo", ">>",
"#187",  ">>",

"uarr",  "\24",
"darr",  "\25",
"rarr",  "\26",
"larr",  "\27", 

"#1028", "\242",
"#1030", "I",
"#1031", "\244",

"#8470", "N",
"bull",  "-", //âîîáùå çäåñü òî÷êà
"percnt","%",

0};

// function to be called to fix the special symbols
void _text::fixSpecial(dword text) {
	byte ch;
	int i , j, z;
	dword aux;
	dword text2 = text;
	loop () {
		ch = ESBYTE[text];
		if (!ch) return;
		if (ch=='&') {
			i = 0;
			j = 0;
			text++;
			while ( i < 7)  {
				ch = ESBYTE[text];
				if (ch == ';') {
					z = get_symbol(#str);
					//debugval("z: ", z);
					if (z == -1) { // not found
						ch = ' ';
					}
					else { // tag found
						aux = unicode_tags[z];
						strtrim(aux);
						debugln(aux);
						ch = ESBYTE[aux];
						// copy the special symbol found
						while (ch) {
							ESBYTE[text2] = ch;
							aux++;
							text2++;
							ch = ESBYTE[aux];
						}
						ch = ESBYTE[text2];
					}
					// clean the old symbol
					while (ch != ';') {
						ESBYTE[text2] = ' ';// should be (char) 0;
						text2++;
						ch = ESBYTE[text2];
					}
					ESBYTE[text2] = ' '; // should be '' or char 0
					break;
				}
				str[i] = ch;
				if (!ch) return;
				text++;
				i++;
			}
			for (i=0; i < 7; i++) str[i] = 0; // clean str
		}
		text++;
		text2++;
	}
}



// function to look for the conversion of special characters
// if not found--> return -1
int get_symbol(char *str2) {
	int i,j;
	//debugln(#str2);
	for (i=0; unicode_tags[i]!=0; i+=2) {
		if (strcmp(str2, unicode_tags[i]) == 0) {
			return (i+1);
		}
	}	
	return -1;
}



