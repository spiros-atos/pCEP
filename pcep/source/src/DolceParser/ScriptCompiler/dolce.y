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
	Grammar Specification-1
*/

%locations
%pure-parser
%lex-param { void* scanner  }
%parse-param { ParserContext* scContext }

%{ 
	#define DEBUG //XXX Remove
	
	#include <assert.h>
	#include <stdio.h>
	
	
	#include "../../Types/TPosLiteral.h"
	#include "../../Types/TAreaLiteral.h"
	#include "../../Types/TTimeLiteral.h"
	#include "../../Types/TDurationLiteral.h"
	
	#include "ASTree.h"
	#include "Parser.h"
	#include "SymbolTable.h"

	#include "ParserContext.h"
	#define scanner scContext->scanner

	using namespace std;

	// Compiled as C++
	// Prototypes to keep the compiler happy
	union YYSTYPE;
	struct YYLTYPE;
	extern int yylex(YYSTYPE* lval, YYLTYPE* loc, void* yyscanner);
	extern void yyerror(YYLTYPE* loc, ParserContext* _context, const char *error);
	

	TDurationLiteral parseDuration(const int _count, const TDurationUnit _unit);
	TDurationLiteral durationUnitAsSeconds(const TDurationUnit _unit);

	
	
%}



/* ----- BEGIN built-in types ------
 *
 *
 */
%token POS AREA CHAR INT FLOAT STRING TIME DURATION
/*
 *
 *
 * ----- END built-in types ------- */


%token AND_EOP OR_EOP AFTER_EOP DURING_EOP ABSENCE_UNARY_EOP


%token EXTERNAL EVENT COMPLEX_EVENT MAP
%token ACCEPT USE LASTS UNTIL
%token PAYLOAD GROUP DETECT WHERE IN
%token SUM_FUNC AVG_FUNC COUNT_FUNC DIFF_FUNC INAREA_FUNC ABS_FUNC FFT_FUNC TRAJ_FUNC COLL_FUNC
%token IDENTIFIER
%token INTEGER_CONSTANT FLOAT_CONSTANT AREA_CONSTANT 
%token STRING_LITERAL CHARACTER_CONSTANT POS_CONSTANT TIME_CONSTANT


%token YEARS MONTHS WEEKS DAYS
%token HOURS MINUTES SECONDS 
/* %token MILLISEC MICROSEC NANOSEC */
%token EQ_OP NE_OP GE_OP GT_OP LE_OP LT_OP
%token OR_OP AND_OP
%token ADD_OP SUB_OP MULT_OP DIV_OP

/*
 * MUST CORRESPOND WITH the union in TValue_ of ../common/Types.h
 */

%union
{
	unsigned long a_number;
	float a_float;
	TAreaLiteral a_area;
	TPosLiteral a_pos;
	TTimeLiteral a_time;
	char a_char;
	char* a_string;
	TDurationUnit a_duration_unit;
	ASTree* a_tree;
} 

	
%type <a_string> IDENTIFIER
%type <a_number> INTEGER_CONSTANT
%type <a_float> FLOAT_CONSTANT
%type <a_area> AREA_CONSTANT
%type <a_pos> POS_CONSTANT
%type <a_char> CHARACTER_CONSTANT
%type <a_string> STRING_LITERAL
%type <a_time> TIME_CONSTANT
%type <a_number> YEARS WEEKS MONTHS DAYS HOURS MINUTES SECONDS 


%type <a_tree> EQ_OP NE_OP GE_OP GT_OP LE_OP LT_OP ABS_FUNC
%type <a_tree> OR_OP AND_OP
%type <a_tree> SUM_FUNC AVG_FUNC COUNT_FUNC DIFF_FUNC INAREA_FUNC

%type <a_tree> identifier postfix_identifier;
%type <a_tree> integer_constant;
%type <a_tree> float_constant;
%type <a_tree> area_constant;
%type <a_tree> pos_constant;
%type <a_tree> time_constant duration_constant;
%type <a_tree> string_literal;
%type <a_tree> character_constant;

%type <a_tree> decl translation_unit external_decl map_decl
%type <a_tree> type_specifier constant_expr
%type <a_tree> payload_definition group_statement detect_statement

%type <a_duration_unit> duration_unit
%type <a_tree> event_prologue event_decl event_units event_unit
%type <a_tree> complex_event_prologue complex_event_decl complex_event_units
%type <a_tree> complex_event_unit
%type <a_tree> accept_statement use_statement lasts_statement 
%type <a_tree> event_attribute_list event_identifier_list
%type <a_tree> event_attribute
%type <a_tree> expression logical_and_expr logical_or_expr comparison_expr
%type <a_tree> primary_expr func_parameter
%type <a_tree> comparison_operator func_expr



%type <a_tree> payload_attribute_list payload_attribute
%type <a_tree> primary_event_expr event_expr
%type <a_tree> event_or_expr event_and_expr event_during_expr event_after_expr 
%type <a_tree> event_absence_expr
%type <a_tree> where_clause window_clause
%type <a_tree> window_range 
/*
%type <a_tree> sliding_time_range 
%type <a_tree> fixed_time_range
%type <a_tree> tuple_range
*/
%type <a_tree> additive_expr
%type <a_tree> multiplicative_expr
%type <a_tree> subtractive_expr
%type <a_tree> divisive_expr
	
%start translation_unit

%%

		
translation_unit  
	: decl 
		{
			
			scContext->parser->getASTree()->addChild($1);
		}
	| translation_unit decl
		{
			scContext->parser->getASTree()->addChild($2);
		}
	;
	



postfix_identifier
	: identifier '.' identifier
		{	$$ = new ASTree(ASTree::PPostfixExpression);
			$$->addChild($3);
			$$->addChild($1);
		}
	| identifier '@' integer_constant '.' identifier
		{	$$ = new ASTree(ASTree::PWindowOffsetPostfixExpression);
			$$->addChild($5);
			$$->addChild($1)->addChild($3);
		}
	| identifier '@' '@' '.' identifier
		{	$$ = new ASTree(ASTree::PWindowOffsetPostfixExpression);
			$$->addChild($5);
			$$->addChild($1)->addChild(new ASTree(
									new TValue((int)0)));
		}

		
identifier
	: IDENTIFIER
		{	$$ = new ASTree(scContext->parser->getSymbolTable()->add(
									$1, 
									scContext->line, 
									scContext->col));
		}
		

integer_constant
	: INTEGER_CONSTANT
		{	$$ = new ASTree(new TValue((int)$1));
		}
		
float_constant
	: FLOAT_CONSTANT
		{	$$ = new ASTree(new TValue($1));
		}
				
area_constant
	: AREA_CONSTANT
		{	$$ = new ASTree(new TValue($1));
		}
	;
		
pos_constant
	: POS_CONSTANT
		{	$$ = new ASTree(new TValue($1));
		}
	;
	
time_constant
	: TIME_CONSTANT
		{	$$ = new ASTree(new TValue($1));
		}
	;

duration_constant
	: INTEGER_CONSTANT duration_unit
		{	$$ = new ASTree(new TValue(parseDuration($1, $2)));
		}
	;
	
character_constant
	: CHARACTER_CONSTANT
		{	$$ = new ASTree(new TValue($1));
		}
	;

string_literal
	: STRING_LITERAL
		{	$$ = new ASTree(new TValue($1));
		}
	;
	
	
decl
	: external_decl
		{ 	$$ = $1; }
	| map_decl
		{	$$ = $1; }
	| event_decl 
		{	$$ = $1; }
	| complex_event_decl
		{	$$ = $1; 	}
	;

	
external_decl
	: EXTERNAL type_specifier identifier '=' constant_expr  ';'
		{	$$ = new ASTree(ASTree::PExternal); 
			$$->addChild($2)->addChild($3);
			$$->addChild($5);
		}


map_decl
	: MAP identifier '=' integer_constant  ';'
		{	$$ = new ASTree(ASTree::PMap); 
			$$->addChild($2);
			$$->addChild($4);
		}
		

event_decl
	: event_prologue '{' event_units'}'
		{ 	$$ = new ASTree(ASTree::PEvent);
			$$->addChild($3);
			$$->addChild($1);
		}
	| event_prologue '{' '}'
		{ 	$$ = new ASTree(ASTree::PEvent);
			$$->addChild($1);
		}
	;
	

event_prologue
	: EVENT identifier
		{ 	$$ = $2;
		}
/* FUTURE
	| EVENT identifier ':' identifier
		{	$$ = $2;
			ASTree* superEvt = $$->addChild(new ASTree(ASTree::PEvent));
			superEvt->addChild($4);
		}
*/
	;


event_units
	: event_unit ';'
		{	$$ = new ASTree(ASTree::PEventUnits);
			$$->addChild($1);
		}
	| event_units event_unit ';' 
		{	$$ = $1;	
			$$->addChild($2);
		}
	;


event_unit
	: accept_statement	 
		{ $$ = $1; }
	| use_statement			
		{ 	$$ = new ASTree(ASTree::PUse);
			$$->addChild($1);
		}
	| lasts_statement
		{ $$ = $1; }
/*FUTURE
	| delegate_decl	 
		{ $$ = $1; }
*/
	;
	

complex_event_decl
	: complex_event_prologue '{' complex_event_units '}'
		{ 	$$ = new ASTree(ASTree::PComplexEvent);
			$$->addChild($3);
			$$->addChild($1);
		}
	| complex_event_prologue '{' '}'
		{ 	$$ = new ASTree(ASTree::PComplexEvent);
			$$->addChild($1);
		}
	;
	

complex_event_prologue
	: COMPLEX_EVENT identifier
		{ 	$$ = $2;
		}
	;


complex_event_units
	: complex_event_unit ';'
		{	$$ = new ASTree(ASTree::PComplexEventUnits);
			$$->addChild($1);
		}
	| complex_event_units complex_event_unit ';' 
		{	$$ = $1;
			$$->addChild($2);
		}
	;

	
complex_event_unit
	: payload_definition
		{	$$ = $1;	}
	| group_statement
		{	$$ = $1;	}
	| detect_statement
		{	$$ = $1;	}
	| lasts_statement
		{ 	$$ = $1; 	}
	;

	
use_statement
	: USE '{' event_attribute_list '}' 
		{ 	$$ = $3;	}
	;
	
lasts_statement
	: LASTS identifier
		{ 	$$ = new ASTree(ASTree::PLasts);
			$$->addChild($2);
		}
	| LASTS duration_constant
		{ 	$$ = new ASTree(ASTree::PLasts);
			$$->addChild($2);
		}
	| LASTS UNTIL event_identifier_list
		{ 	$$ = new ASTree(ASTree::PLasts);
			$$->addChild($3);
		}
	;
	
	
event_identifier_list
	: identifier
		{ 	$$ = new ASTree(ASTree::PEventIdentifierList);
			$$->addChild($1);
		}
	| event_identifier_list ',' identifier
		{	$$ = $1;
			$$->addChild($3);
		}
	;
	
event_attribute_list
	: event_attribute
		{ 	$$ = new ASTree(ASTree::PEventAttributeList);
			$$->addChild($1);
		}
	| event_attribute_list ',' event_attribute
		{	$$ = $1;
			$$->addChild($3);
		}
	;

event_attribute
	: type_specifier identifier
		{	$$ = new ASTree(ASTree::PEventAttribute);
			$$->addChild($1)->addChild($2);
		}
/* FUTURE	
delegate_decl
	: DELEGATE
		{	$$ = new ASTree(ASTree::PDelegate); }
	;
*/
	
accept_statement
	: ACCEPT '{' expression '}' 
		{ 	$$ = new ASTree(ASTree::PAccept);
			$$->addChild($3);
		}
	;
	
payload_definition
	: PAYLOAD '{' payload_attribute_list '}'
		{	$$ = $3;
		}
	;


payload_attribute_list
	: payload_attribute
		{ 	$$ = new ASTree(ASTree::PPayload);
			$$->addChild($1);
		}
	| payload_attribute_list ',' payload_attribute
		{	$$ = $1;
			$$->addChild($3);
		}
	;


payload_attribute
	:  type_specifier identifier '=' expression
		{	$$ = new ASTree(ASTree::PPayloadAttribute);
			$$->addChild($1)->addChild($2);
			$$->addChild($4);
		}
	;
	
	
group_statement
	: GROUP postfix_identifier
		{	$$ = new ASTree(ASTree::PGroup); 
			$$->addChild($2);
		}
	| GROUP identifier
		{	$$ = new ASTree(ASTree::PGroup); 
			$$->addChild($2);
		}
	;
	
detect_statement
	: DETECT event_expr where_clause window_clause
		{ 	$$ = new ASTree(ASTree::PDetect);
			$$->addChild($2);
			
			if($3)
				$$->addChild($3);
				
			if($4)
				$$->addChild($4);
		}
	;

	
where_clause
	: /* EMPTY */
		{	$$ = 0;
		}
	| WHERE expression
		{	$$ = new ASTree(ASTree::PDetectWhere);
			$$->addChild($2);
		}
	;
	
window_clause
	: /* EMPTY */
		{	$$ = 0;
		}
	| IN '[' window_range ']'
		{	$$ = new ASTree(ASTree::PDetectWindow);
			$$->addChild($3);
		}
	;
	
	
primary_event_expr
	: identifier
		{	$$ = $1;	
		}
	| '(' event_expr ')'
		{	$$ = $2;	}
	;

event_expr
	: event_or_expr
		{	
			$$ = new ASTree(ASTree::PDetectEventExpression);
			$$->addChild($1);
		}
	;
	
event_or_expr
	: event_and_expr
		{	$$ = $1;	}
	| event_or_expr OR_EOP event_and_expr
		{	$$ = new ASTree(ASTree::PEventOpOr);
			$$->addChild($1);
			$$->addChild($3);
		}
	;
	
event_and_expr
	: event_during_expr
		{	$$ = $1;	}
	| event_and_expr AND_EOP event_during_expr
		{	$$ = new ASTree(ASTree::PEventOpAnd);
			$$->addChild($1);
			$$->addChild($3);
		}
	;
	
event_during_expr
	: event_after_expr
		{	$$ = $1;	}
	| event_during_expr DURING_EOP event_after_expr
		{	$$ = new ASTree(ASTree::PEventOpDuring);
			$$->addChild($1);
			$$->addChild($3);
		}
	;

event_after_expr
	: event_absence_expr
		{	$$ = $1;	}
	| event_after_expr AFTER_EOP event_absence_expr
		{	$$ = new ASTree(ASTree::PEventOpAfter);
			$$->addChild($1);
			$$->addChild($3);
		}
	;

event_absence_expr
	: primary_event_expr
		{	$$ = $1;	}
	| ABSENCE_UNARY_EOP event_absence_expr
		{	$$ = new ASTree(ASTree::PEventOpAbsence);
			$$->addChild($2);
		}
	;
	
	

constant_expr
	: integer_constant		/* INT */
		{	$$ = $1;	}
	| float_constant		/* FLOAT */
		{	$$ = $1; 	}
	| pos_constant			/* POS */
		{	$$ = $1;	}
	| area_constant			/* AREA */
		{	$$ = $1;	}
	| string_literal		/* STRING */
		{	$$ = $1;	}
	| character_constant	/* CHAR */
		{	$$ = $1;	}
	| time_constant			/* TIME */
		{	$$ = $1;	}
	| duration_constant		/* DURATION */
		{	$$ = $1;	}
;

	
primary_expr
	: constant_expr
		{ 	$$ = $1; 
		}
	| identifier
		{	$$ = $1; 
		}
	| postfix_identifier 
		{ 	$$ = $1; 
		}
	| '(' expression ')'
		{	$$ = $2; 
		}
	;

expression
	: logical_or_expr
		{ 	$$ = $1; }
	;
	

logical_or_expr
	: logical_and_expr
		{ 	$$ = $1; }
	| logical_or_expr OR_OP logical_and_expr
		{	$$ = new ASTree(ASTree::PLogicalOrOp);
			$$->addChild($1);
			$$->addChild($3);
		}
	;

	
logical_and_expr
	: comparison_expr
		{ 	$$ = $1; }
	| logical_and_expr AND_OP comparison_expr
		{	$$ = new ASTree(ASTree::PLogicalAndOp);
			$$->addChild($1);
			$$->addChild($3);
		}
	;
	
	
	
comparison_expr
	: additive_expr
		{ 	$$ = $1; }
	| comparison_expr comparison_operator additive_expr
		{	$$ = $2;
			$$->addChild($1);
			$$->addChild($3);
		}
	;
	
	
additive_expr
	: subtractive_expr
		{	$$ = $1; }
	| additive_expr ADD_OP subtractive_expr
		{	$$ = new ASTree(ASTree::POpAdd);
			$$->addChild($1);
			$$->addChild($3);
		}
	;
	
	
	
subtractive_expr
	: multiplicative_expr
		{	$$ = $1; }
	| subtractive_expr SUB_OP multiplicative_expr
		{	$$ = new ASTree(ASTree::POpSub);
			$$->addChild($1);
			$$->addChild($3);
		}
	;
	
multiplicative_expr
	: divisive_expr
		{	$$ = $1; }
	| multiplicative_expr MULT_OP divisive_expr
		{	$$ = new ASTree(ASTree::POpMult);
			$$->addChild($1);
			$$->addChild($3);
		}
	;


divisive_expr
	: func_expr
		{	$$ = $1; }
	| divisive_expr DIV_OP func_expr
		{	$$ = new ASTree(ASTree::POpDiv);
			$$->addChild($1);
			$$->addChild($3);
		}
	;	
	


func_expr
	: primary_expr
		{ $$ = $1; }
	| SUM_FUNC '(' func_parameter ')'
		{ 	$$ = new ASTree(ASTree::PSumFunc);
			$$->addChild($3);
		}
	| AVG_FUNC '(' func_parameter ')'
		{ 	$$ = new ASTree(ASTree::PAvgFunc);
			$$->addChild($3);
		}
	| COUNT_FUNC '(' func_parameter ')'
		{ 	$$ = new ASTree(ASTree::PCountFunc);
			$$->addChild($3);
		}
	| INAREA_FUNC '(' expression ',' expression ')'
		{ 	$$ = new ASTree(ASTree::PInAreaFuncX);
			$$->addChild($3);
                        $$->addChild($5);
		}
	| DIFF_FUNC '(' expression ',' expression ')'
		{ 	$$ = new ASTree(ASTree::PDiffFuncX);
			$$->addChild($3);
			$$->addChild($5);
		}
	| DIFF_FUNC '(' func_parameter ')'
		{ 	$$ = new ASTree(ASTree::PDiffFunc);
			$$->addChild($3);
		}
	| ABS_FUNC '(' expression ')'
		{ 	$$ = new ASTree(ASTree::PAbsFunc);
			$$->addChild($3);
		}
	| FFT_FUNC '(' expression ')'
		{ 	$$ = new ASTree(ASTree::PFftFunc);
			$$->addChild($3);
		}
	| TRAJ_FUNC '(' expression ')'
		{ 	$$ = new ASTree(ASTree::PTrajFunc);
			$$->addChild($3);
		}
	| COLL_FUNC '(' expression ')'
		{ 	$$ = new ASTree(ASTree::PCollFunc);
			$$->addChild($3);
		}
	;
	
func_parameter
	: identifier
		{	$$ = $1; 
		}
	| postfix_identifier
		{	$$ = $1; 
		}
	;
		
	

comparison_operator
	: EQ_OP		{ $$ = new ASTree(ASTree::POpEq); }
	| NE_OP		{ $$ = new ASTree(ASTree::POpNe); }
	| GE_OP		{ $$ = new ASTree(ASTree::POpGe); }
	| GT_OP		{ $$ = new ASTree(ASTree::POpGt); }
	| LE_OP		{ $$ = new ASTree(ASTree::POpLe); }
	| LT_OP		{ $$ = new ASTree(ASTree::POpLt); }
	;


/*
	Years, Months, Weeks Days, hours, minutes, seconds
*/
duration_unit
	: YEARS		{ $$ = d_years; }
	| MONTHS	{ $$ = d_months; }
	| WEEKS		{ $$ = d_weeks; }
	| DAYS		{ $$ = d_days; }
	| HOURS		{ $$ = d_hours; }
	| MINUTES	{ $$ = d_minutes; }
	| SECONDS	{ $$ = d_seconds; }
	;
	
type_specifier
	: POS		{ $$ = new ASTree(ASTree::PPos); }
	| AREA		{ $$ = new ASTree(ASTree::PArea); }
	| CHAR		{ $$ = new ASTree(ASTree::PChar); }
	| INT		{ $$ = new ASTree(ASTree::PInt); }
	| FLOAT		{ $$ = new ASTree(ASTree::PFloat); }
	| STRING	{ $$ = new ASTree(ASTree::PString); }
	| TIME		{ $$ = new ASTree(ASTree::PTime); }
	| DURATION 	{ $$ = new ASTree(ASTree::PDuration); }
	;
	
		

window_range
	: identifier					/* TUPLE / SLIDING */
		{	$$ = $1;	}
	| duration_constant 					/* SLIDING */
		{	$$ = new ASTree(ASTree::PSlidingTimeWin);
			$$->addChild($1);
		}
	| time_constant ',' duration_constant	/* FIXED */
		{ 	$$ = new ASTree(ASTree::PFixedTimeWin);
			$$->addChild($1);
			$$->addChild($3);
		}
	| identifier ',' duration_constant		/* FIXED */
		{ 	$$ = new ASTree(ASTree::PFixedTimeWin);
			$$->addChild($1);
			$$->addChild($3);
		}
	| identifier ',' identifier 			/* FIXED */
		{ 	$$ = new ASTree(ASTree::PFixedTimeWin);
			$$->addChild($1);
			$$->addChild($3);
		}
	| integer_constant						/* TUPLE */
		{	$$ = new ASTree(ASTree::PTupleWin);
			$$->addChild($1);
		}
	| integer_constant identifier			/* TUPLE */
		{	$$ = new ASTree(ASTree::PTupleWin);
			$$->addChild($1);
			$$->addChild($2);
		}
	| identifier identifier					/* TUPLE */
		{	$$ = new ASTree(ASTree::PTupleWin);
			$$->addChild($1);
			$$->addChild($2);
		}
	;

%%


#include <stdio.h>

extern char yytext[];




TDurationLiteral parseDuration(const int _count, const TDurationUnit _unit)
{
	return (time_t)(_count * durationUnitAsSeconds(_unit));
}


//FIXME: add special prefix to the functions in order to establish a namespace  (e.g. "dolce_l_"
TDurationLiteral durationUnitAsSeconds(const TDurationUnit _unit)
{
	if(_unit == d_years) return (time_t)31536000;
	if(_unit == d_months) return (time_t)2628000; //based on month == 365/12 days.
	if(_unit == d_weeks) return (time_t)604800;
	if(_unit == d_days) return (time_t)86400;
	if(_unit == d_hours) return (time_t)3600;
	if(_unit == d_minutes) return (time_t)60;
	if(_unit == d_seconds) return (time_t)1;
	
	return (time_t)0;
}



