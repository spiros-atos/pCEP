/*
 * Copyright (C) 2019  Atos Spain SA. All rights reserved.
 * 
 * This file is part of pCEP.
 * 
 * pCEP is free software: you can redistribute it and/or modify it under the 
 * terms of the Apache License, Version 2.0 (the License);
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * The software is provided "AS IS", without any warranty of any kind, express or implied,
 * including but not limited to the warranties of merchantability, fitness for a particular
 * purpose and noninfringement, in no event shall the authors or copyright holders be 
 * liable for any claim, damages or other liability, whether in action of contract, tort or
 * otherwise, arising from, out of or in connection with the software or the use or other
 * dealings in the software.
 * 
 * See README file for the full disclaimer information and LICENSE file for full license 
 * information in the project root.
 * 
 * Authors:  Atos Research and Innovation, Atos SPAIN SA
 */


/*
	Dolce - DescriptOn Language for Complex Events.
	Lexical Tokens for Grammar Specification-1
	

	TODO: Reconsider the {YY}, {MM} etc. tokens and either
		- raise specialized syntax err in parseTime()
		- consider any out-of-range time values as semantic errors (not lexing errors)
		
	TODO: Same as the above, but for geographic constants.
		
*/
%option reentrant
%option bison-bridge
%option bison-locations
%option noyywrap

L	[a-zA-Z_]
D	[0-9]
F	-?{D}+\.{D}+

YY	{D}{D}{D}{D}			
MM	0?[1-9]|1[012]			
DD	0?[1-9]|[12][0-9]|3[01]	

hh	[0-9]|[01][0-9]|2[0123]
mm	[0-5][0-9]				
ss	[0-5][0-9]				



%{	

#include "ParserContext.h"

#include <stdio.h>
#include "../DolceCompilerCodes.h"
#include "../../Types/TPosLiteral.h"
#include "../../Types/TAreaLiteral.h"
#include "../../Types/TTimeLiteral.h"
#include "../../Types/TDurationLiteral.h"
#include "Parser.h"
#include "ASTree.h"
#include "ErrorList.h"


#include "dolce.y.tab.hpp"

void count(yyscan_t yyscanner);
void countline(yyscan_t yyscanner);
void eat_star_comments(yyscan_t yyscanner);
void eat_line_comments(yyscan_t yyscanner);
TPosLiteral parsePos(yyscan_t yyscanner);
TAreaLiteral parseArea(yyscan_t yyscanner, const int _mode);
void parseGeographicLiterals(yyscan_t yyscanner, float* _list_, const int _count);
TTimeLiteral parseTime(yyscan_t yyscanner, const int _mode);
char parseChar(yyscan_t yyscanner);
char* parseString(yyscan_t yyscanner);
void countNewLine(yyscan_t yyscanner, char c);
TDurationLiteral parseDuration(const int _count, const TDurationUnit _unit);
TDurationLiteral durationUnitAsSeconds(const TDurationUnit _unit);

/* FUTURE
"ms"				{ return MILLISEC; }
"us"				{ return MICROSEC; }
"ns"				{ return NANOSEC; }
"delegate"			{ return DELEGATE; }
*/

%} 


%%
"/*"			{ eat_star_comments(yyscanner); }
"//"			{ eat_line_comments(yyscanner); }



"external"			{ return EXTERNAL; }
"map"				{ return MAP; }
"event"			 	{ return EVENT; }
"complex"			{ return COMPLEX_EVENT; }
"accept"	  		{ return ACCEPT; }
"duration"			{ return DURATION; }
"time"				{ return TIME; }

"and"				{ return AND_EOP; }
"or"				{ return OR_EOP; }
"after"				{ return AFTER_EOP; }
"during"			{ return DURING_EOP; }
"!"					{ return ABSENCE_UNARY_EOP; }

"use"				{ return USE; }
"lasts"				{ return LASTS; }
"until"				{ return UNTIL; }

"pos"				{ return POS; }
"area"				{ return AREA; }
"char"				{ return CHAR; }
"string"			{ return STRING; }
"int"				{ return INT; }
"float"				{ return FLOAT; }
"year"s?			{ return YEARS; }
"week"s?			{ return WEEKS; }
"month"s?			{ return MONTHS; } 
"day"s?				{ return DAYS; }
"hour"s?			{ return HOURS; }
"minute"s?			{ return MINUTES; }
"second"s?			{ return SECONDS; }


"payload"			{ return PAYLOAD; }
"group"				{ return GROUP; }
"detect"			{ return DETECT; }
"where"				{ return WHERE; }
"in"				{ return IN; }

"sum"				{ return SUM_FUNC; }
"avg"				{ return AVG_FUNC; }
"count"				{ return COUNT_FUNC; }
"diff"				{ return DIFF_FUNC; }
"inarea"			{ return INAREA_FUNC; }
"abs"				{ return ABS_FUNC; }
"fft"				{ return FFT_FUNC; }
"traj"				{ return TRAJ_FUNC; }
"coll"				{ return COLL_FUNC; }

"&&"				{ return AND_OP; }
"||"				{ return OR_OP; }

"=="				{ return EQ_OP; }
"!="				{ return NE_OP; }
">="				{ return GE_OP; }
">"					{ return GT_OP; }
"<="				{ return LE_OP; }
"<"					{ return LT_OP; }

"+"					{ return ADD_OP; }
"-"					{ return SUB_OP; }
"*"					{ return MULT_OP; }
"/"					{ return DIV_OP; }

";"					{ return (';'); }
"{"					{ return ('{'); }
"}"					{ return ('}'); }
"("					{ return ('('); }
")"					{ return (')'); }
"["					{ return ('['); }
"]"					{ return (']'); }
":"					{ return (':'); }
","					{ return (','); }
"."					{ return ('.'); }
"$"					{ return ('$'); }
"@"					{ return ('@'); }
"="					{ return ('='); }

{L}({L}|{D})*		{ yylval->a_string = yytext; 
						return IDENTIFIER;
					}
-?{D}+				{ yylval->a_duration_unit = (TDurationUnit)atoi(yytext);
						return INTEGER_CONSTANT;
					}

{F}					{ yylval->a_float = atof(yytext);
						return FLOAT_CONSTANT;
					}			

\"([^\"\\]|\\.)*\"	{	yylval->a_string = parseString(yyscanner); 
						return STRING_LITERAL;
					}		

'([^'\\]|\\.)'		{	yylval->a_char = parseChar(yyscanner);
						return CHARACTER_CONSTANT;
					}		

{hh}:{mm}(:{ss})?	{ 
						yylval->a_time = parseTime(yyscanner, 0);
						return TIME_CONSTANT;
					}
					
({YY}-)?{MM}-{DD}	{ 
						yylval->a_time = parseTime(yyscanner, 1);
						return TIME_CONSTANT;
					}
					
({YY}-)?{MM}-{DD}@{hh}:{mm}(:{ss})?	{ 
						yylval->a_time = parseTime(yyscanner, 2);
						return TIME_CONSTANT;
					}
					
{F}\\{F}			{ 	yylval->a_pos = parsePos(yyscanner);
						return POS_CONSTANT;
					}
					
{F}\\{F}\\{F}		{ yylval->a_area = parseArea(yyscanner, 0);
						return AREA_CONSTANT;
					}

{F}\\{F}\\{F}\\{F}	{ yylval->a_area = parseArea(yyscanner, 1);
						return AREA_CONSTANT;
					}

					
[\n\r]				{ countline(yyscanner);  }
[ \t\v\f]			;	/* ignore whitespace */


%%


void yyerror(YYLTYPE* loc, ParserContext* _context, const char* _msg)
{
	_context->parser->mErrorList->addError(
								loc->first_line,
								loc->first_column,
								"%d: '%s'",
								DMSG_SYNTAX_ERROR,
								_msg);
	
}



void count(yyscan_t yyscanner)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
	YYLTYPE* loc = yyget_lloc(yyscanner);
	
	int i;
	char c;

	
	for (i = 0; (c = yytext[i]) != '\0'; i++)
	{
		if((c == '\n') || (c == '\r'))
		{
			// Make sure we do not recount when CR-LF
			if(c == '\r')
			{
				i++;
				if((c != '\n') && (c != '\0'))
					i--;
			}
			
			loc->first_line++;
			loc->first_column = 0;
		}
		else if (yytext[i] == '\t')
			loc->first_column += 8 - (loc->first_column % 8);
		else
			loc->first_column++;
	}
}

void countline(yyscan_t yyscanner)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	char c = yytext[0];
	
	countNewLine(yyscanner, c);
}


TPosLiteral parsePos(yyscan_t yyscanner)
{
	float list[2];
	
	parseGeographicLiterals(yyscanner, list,  2);

	TPosLiteral ret;
	
	ret.lat = list[0];
	ret.lon = list[1];

#ifdef CEP_DEBUG
	printf("lat=%f,lon=%f\n", list[0], list[1]); fflush(stdout);
#endif

	return ret;
}



/* mode: 0-circular, 1-rectangular */
TAreaLiteral parseArea(yyscan_t yyscanner, const int _mode)
{
	float list[4];
	
	int max = (_mode == TAreaLiteral::KAreaCircular) ? 3 : 4;
	
	parseGeographicLiterals(yyscanner, list,  max);

	TAreaLiteral ret;
	
	ret.type = _mode;
	ret.lat = list[0];
	ret.lon = list[1];
	ret.lat2 = list[2];
	
	if(_mode == TAreaLiteral::KAreaCircular)
		ret.radius = list[2];
	else
		ret.lon2 = list[3];

#ifdef CEP_DEBUG		
	if(_mode == TAreaLiteral::KAreaCircular)
		printf("lat=%f,lon=%f,radius=%f\n", list[0], list[1], list[2]); fflush(stdout);
	if(_mode == TAreaLiteral::KAreaRectangular)
		printf("lat=%f,lon=%f,lat=%f,lon=%f\n", list[0], list[1], list[2], list[3]); fflush(stdout);
#endif
	
	return ret;
}


	
void parseGeographicLiterals(yyscan_t yyscanner, float* _list_, const int _count)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	/*  
		So yes, we're manually lexing within the lexer, but we REALLY want
		these as built-in types, i.e. as literals, not a composition of tokens to 
		parsed upstream as single grammar elements - ML
	*/
	char* tok = (char*)malloc(strlen(yytext) + 1);  /* avoid buffer overflow (security) */
	char* tokBegin = tok;
	char c;
	
	int idx = 0;

	while((c = *yytext) != '\0')
	{
		if((c == '\\') || (c == '\0'))
		{
			*tok = '\0';
			tok = tokBegin;
			if(idx < _count)
				_list_[idx++] = atof(tok);
				
			yytext++;
		}
		else
		{
			*tok++ = *yytext++;
			*tok = 0;
		}
	}
	*tok = '\0';
	tok = tokBegin;
	
	if(idx < _count)
		_list_[idx++] = atof(tokBegin);
		
	free(tok);
}


/* _mode: 0-only time, 1-date+time */
TTimeLiteral parseTime(yyscan_t yyscanner, const int _mode)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	#ifdef CEP_DEBUG
	printf("\nZTIME DETECTED (%d)\n", _mode); 
#endif
	char* tok = (char*)malloc(strlen(yytext) + 1);  /* avoid buffer overflow */
	char* tokBegin = tok;
	char c;
	
	int idx = 0;
	int _count = 3;
	char val[3] = { 0,0,0 };

	while((c = *yytext) != '\0')
	{
		if((c == ':') || (c == '\0'))
		{
			*tok = '\0';
			tok = tokBegin;
			if(idx < _count)
				val[idx++] = atoi(tok);
				
			yytext++;
		}
		else
		{
			*tok++ = *yytext++;
			*tok = 0;
		}
	}
	*tok = '\0';
	tok = tokBegin;
	
	if(idx < _count)
		val[idx++] = atoi(tok);
		
	free(tok);
	
	TTimeLiteral tm;
	tm.time.tm_hour=val[0];
	tm.time.tm_min=val[1];
	tm.time.tm_sec=val[2];
	
#ifdef CEP_DEBUG
	printf("\n****** TIME = %d:%d:%d\n", tm.time.tm_hour, tm.time.tm_min, tm.time.tm_sec);
#endif

	return tm;
}



void eat_star_comments(yyscan_t yyscanner)
{
/*


<*>.|\n     		{ fprintf(stderr, "Unrecognized symbol '%s'\n", yytext); }
*/
	char c, tmp;
loop:
	while ((c = yyinput(yyscanner)) != '*' && c != 0)
	{
		countNewLine(yyscanner, c);
	}

	if ((tmp = yyinput(yyscanner)) != '/' && c != 0)
	{
		goto loop;
	}
}

void eat_line_comments(yyscan_t yyscanner)
{

	char c;

	while (((c = yyinput(yyscanner)) != '\n') && (c != '\r')  && (c != 0))
		;
		
	countNewLine(yyscanner, c);
}


void countNewLine(yyscan_t yyscanner, char c)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
	YYLTYPE* loc = yyget_lloc(yyscanner);

	if((c=='\n') || (c=='\r'))
	{
		// make sure we do not double-count in case of CR-LF files
		if(c=='\r')
		{
			/* eat the LF if necessary */
			c = yyinput(yyscanner);
			if((c != '\n') && (c != '\0'))
				unput(c);
		}
		loc->first_line++;
	}
	
}


char parseChar(yyscan_t yyscanner)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	int l = strlen(yytext);

	if(l == 3)
		return yytext[1];
		
	if(l == 4)
		return(yytext[2]);
	
	return 0;
}


char* parseString(yyscan_t yyscanner)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	/* copy yytext, then manipulate yytext directly.  */
	char* orig = (char*)malloc(strlen(yytext) + 1);
	
	strcpy(orig, yytext+1);

	char* yytextBegin = yytext;
	char* origBegin = orig;
	
	while((*orig != '"') && (*orig != '\0'))
	{
		if(*orig == '\\')
			orig++;
		*yytext++ = *orig++;
	}
	*yytext = '\0';
	
	free(origBegin);
	
	return yytextBegin;
}

