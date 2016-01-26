/**
 *  C++ modification
 *  by Tatsuhiko Suzuki < szk@ex-vivo.net >
 */

/*
 *  Below is the original copyright of minischeme 0.85
 */

/*
 *      ---------- Mini-Scheme Interpreter Version 0.85 ----------
 *
 *                coded by Atsushi Moriwaki (11/5/1989)
 *
 *            E-MAIL :  moriwaki@kurims.kurims.kyoto-u.ac.jp
 *
 *               THIS SOFTWARE IS IN THE PUBLIC DOMAIN
 *               ------------------------------------
 * This software is completely free to copy, modify and/or re-distribute.
 * But I would appreciate it if you left my name on the code as the author.
 *
 */

/*-- From TinyScheme
 *
 *  This version has been modified by R.C. Secrist.
 *
 *  Mini-Scheme is now maintained by Akira KIDA.
 *
 *  This is a revised and modified version by Akira KIDA.
 *	current version is 0.85k4 (15 May 1994)
 *
 *  Please send suggestions, bug reports and/or requests to:
 *		<SDI00379@niftyserve.or.jp>
 *--
 */

/*
 * Here is System declaration.
 * Please define exactly one symbol in the following section.
 */

namespace scm
{
/*
 * Define or undefine following symbols as you need.
 */
// #define VERBOSE	// define this if you want verbose GC
#pragma warning(disable: 4710) // not inlined

/*--
 *  If your machine can't support "forward single quotation character"
 *  i.e., '`',  you may have trouble to use backquote.
 *  So use '^' in place of '`'.
 */
#define BACKQUOTE '`'

#ifdef SMALL
#define CELL_SEGSIZE  2048
#define CELL_NSEGMENT  100
#define STR_SEGSIZE   2048
#define STR_NSEGMENT   100
#else
#define CELL_SEGSIZE    5000	// # of cells in one segment
#define CELL_NSEGMENT   100	// # of segments for cells
#define STR_SEGSIZE     2500	// bytes of one string segment
#define STR_NSEGMENT    100	// # of segments for strings
#endif

// #define banner "Hello, This is Mini-Scheme Interpreter Version 0.85k4-a.\n"
#define SYSV 1
#define banner "This is mstmpl.hpp\n"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define prompt "> "
#define InitFile "init.scm"

// operator code
#define	OP_LOAD		0
#define	OP_T0LVL	1
#define	OP_T1LVL	2
#define	OP_READ		3
#define	OP_VALUEPRINT	4
#define	OP_EVAL		5
#define	OP_E0ARGS	6
#define	OP_E1ARGS	7
#define	OP_APPLY	8
#define	OP_DOMACRO	9

#define	OP_LAMBDA	10
#define	OP_QUOTE	11
#define	OP_DEF0		12
#define	OP_DEF1		13
#define	OP_BEGIN	14
#define	OP_IF0		15
#define	OP_IF1		16
#define	OP_SET0		17
#define	OP_SET1		18
#define	OP_LET0		19
#define	OP_LET1		20
#define	OP_LET2		21
#define	OP_LET0AST	22
#define	OP_LET1AST	23
#define	OP_LET2AST	24
#define	OP_LET0REC	25
#define	OP_LET1REC	26
#define	OP_LET2REC	27
#define	OP_COND0	28
#define	OP_COND1	29
#define	OP_DELAY	30
#define	OP_AND0		31
#define	OP_AND1		32
#define	OP_OR0		33
#define	OP_OR1		34
#define	OP_C0STREAM	35
#define	OP_C1STREAM	36
#define	OP_0MACRO	37
#define	OP_1MACRO	38
#define	OP_CASE0	39
#define	OP_CASE1	40
#define	OP_CASE2	41

#define	OP_PEVAL	42
#define	OP_PAPPLY	43
#define	OP_CONTINUATION	44
#define	OP_ADD		45
#define	OP_SUB		46
#define	OP_MUL		47
#define	OP_DIV		48
#define	OP_REM		49
#define	OP_CAR		50
#define	OP_CDR		51
#define	OP_CONS		52
#define	OP_SETCAR	53
#define	OP_SETCDR	54
#define	OP_NOT		55
#define	OP_BOOL		56
#define	OP_NULL		57
#define	OP_ZEROP	58
#define	OP_POSP		59
#define	OP_NEGP		60
#define	OP_NEQ		61
#define	OP_LESS		62
#define	OP_GRE		63
#define	OP_LEQ		64
#define	OP_GEQ		65
#define	OP_SYMBOL	66
#define	OP_NUMBER	67
#define	OP_STRING	68
#define	OP_PROC		69
#define	OP_PAIR		70
#define	OP_EQ		71
#define	OP_EQV		72
#define	OP_FORCE	73
#define	OP_WRITE	74
#define	OP_DISPLAY	75
#define	OP_NEWLINE	76
#define	OP_ERR0		77
#define	OP_ERR1		78
#define	OP_REVERSE	79
#define	OP_APPEND	80
#define	OP_PUT		81
#define	OP_GET		82
#define	OP_QUIT		83
#define	OP_GC		84
#define	OP_GCVERB	85
#define	OP_NEWSEGMENT	86

#define	OP_RDSEXPR	87
#define	OP_RDLIST	88
#define	OP_RDDOT	89
#define	OP_RDQUOTE	90
#define	OP_RDQQUOTE	91
#define	OP_RDUNQUOTE	92
#define	OP_RDUQTSP	93

#define	OP_P0LIST	94
#define	OP_P1LIST	95

#define	OP_LIST_LENGTH	96
#define	OP_ASSQ		97
#define	OP_PRINT_WIDTH	98
#define	OP_P0_WIDTH	99
#define	OP_P1_WIDTH	100
#define	OP_GET_CLOSURE	101
#define	OP_CLOSUREP	102
#define	OP_MACROP	103

#define T_STRING         1 /* 0000000000000001 */
#define T_NUMBER         2 /* 0000000000000010 */
#define T_SYMBOL         4 /* 0000000000000100 */
#define T_SYNTAX         8 /* 0000000000001000 */
#define T_PROC          16 /* 0000000000010000 */
#define T_PAIR          32 /* 0000000000100000 */
#define T_CLOSURE       64 /* 0000000001000000 */
#define T_CONTINUATION 128 /* 0000000010000000 */
#define T_MACRO        256 /* 0000000100000000 */
#define T_PROMISE      512 /* 0000001000000000 */
#define T_ATOM       16384 /* 0100000000000000 */	/* only for gc */
#define CLRATOM      49151 /* 1011111111111111 */	/* only for gc */
#define MARK         32768 /* 1000000000000000 */
#define UNMARK       32767 /* 0111111111111111 */

class Cell
{
public:
    Cell(void) {}
    virtual ~Cell(void) {}

    inline unsigned long type(void) { return flag; }
    inline void set_type(unsigned long f_) { flag = f_; }
    inline const bool isstring(void) { return flag & T_STRING; }
    inline char* strvalue(void) { return Obj.Str.svalue; }
    inline void set_strvalue(char* str_) { Obj.Str.svalue = str_; }
    inline short keynum(void) { return Obj.Str.keynum; }
    inline void set_keynum(short n_) { Obj.Str.keynum = n_; }

    inline const bool isnumber(void) { return flag & T_NUMBER; }
    inline long numvalue(void) { return Obj.Number.ivalue; }
    inline void set_numvalue(long n_) { Obj.Number.ivalue = n_; }
    inline const bool isreal(void) { return flag & T_NUMBER; }
    inline const float realvalue(void) { return Obj.Real.rvalue; }
    inline void set_real(float num_) { Obj.Real.rvalue = num_;}

    inline const bool ispair(void) { return flag & T_PAIR; }
    inline Cell* car(void) { return Obj.Cons.car; }
    inline void set_car(Cell* car_) { Obj.Cons.car = car_; }
    inline Cell* cdr(void) { return Obj.Cons.cdr; }
    inline void set_cdr(Cell* cdr_) { Obj.Cons.cdr = cdr_; }

    inline const bool issymbol(void) { return flag & T_SYMBOL; }
    inline char* symname(void) { return car()->strvalue(); }
    inline const bool hasprop(void) { return flag & T_SYMBOL; }
    inline Cell* symprop(void) { return cdr(); }
    inline void set_symprop(Cell* c_) { set_cdr(c_); }

    inline const bool issyntax(void) { return flag & T_SYNTAX; }
    inline const bool isproc(void) { return flag & T_PROC; }
    inline char* syntaxname(void) { return car()->strvalue(); }
    inline short syntaxnum(void) { return car()->keynum(); }
    inline void set_syntaxnum(short n_) { car()->set_keynum(n_); }
    inline long procnum(void) { return numvalue(); }

    inline const bool isclosure(void) { return flag & T_CLOSURE; }
    inline const bool ismacro(void) { return flag & T_MACRO; }
    inline Cell* closure_code(void) { return car(); }
    inline Cell* closure_env(void) { return cdr(); }

    inline const bool iscontinuation(void) { return flag & T_CONTINUATION; }
    inline Cell* cont_dump(void) { return cdr(); }
    inline void set_cont_dump(Cell* dump) { set_cdr(dump); }

    inline const bool ispromise(void) { return flag & T_PROMISE; }
    inline unsigned long& setpromise(void) { return flag |= T_PROMISE; }

    inline const bool isatom(void) { return flag & T_ATOM; }
    inline unsigned long& setatom(void) { return flag |= T_ATOM; }
    inline unsigned long& clratom(void) { return flag &= CLRATOM; }

    inline const bool ismark(void) { return flag & MARK; }
    inline unsigned long& setmark(void) { return flag |= MARK; }
    inline unsigned long& clrmark(void) { return flag &= UNMARK; }

    inline const bool isabbrev(Cell* nil_) { return ispair() && cdr() == nil_; }

    inline Cell* caar(void) { return car()->car(); }
    inline void set_caar(Cell* c_) { car()->set_car(c_); }
    inline Cell* cadr(void) { return cdr()->car(); }
    inline void set_cadr(Cell* c_) {  cdr()->set_car(c_); }
    inline Cell* cdar(void) { return car()->cdr(); }
    inline void set_cdar(Cell* c_) { car()->set_cdr(c_); }
    inline Cell* cddr(void) { return cdr()->cdr(); }
    inline void set_cddr(Cell* c_) { cdr()->set_cdr(c_); }
    inline Cell* cadar(void) { return car()->cdr()->car(); }
    inline void set_cadar(Cell* c_) { car()->cdr()->set_car(c_); }
    inline Cell* caddr(void) { return cdr()->cdr()->car(); }
    inline void set_caddr(Cell* c_) { cdr()->cdr()->set_car(c_); }
    inline Cell* cadaar(void) { return car()->car()->cdr()->car(); }
    inline void set_cadaar(Cell* c_) { car()->car()->cdr()->set_car(c_); }
    inline Cell* cadddr(void) { return cdr()->cdr()->cdr()->car(); }
    inline void set_cadddr(Cell* c_) { cdr()->cdr()->cdr()->set_car(c_); }
    inline Cell* cddddr(void) { return cdr()->cdr()->cdr()->cdr(); }
    inline void set_cddddr(Cell* c_) { cdr()->cdr()->cdr()->set_cdr(c_); }

private:
    unsigned long flag;
    union
    {
        struct { char* svalue; short keynum; } Str;
        struct { long ivalue; } Number;
        struct { float rvalue; } Real;
        struct { Cell* car,* cdr; } Cons;
    } Obj;

};

typedef class Cell* pointer;

template<class NumT, class StrT> class Scm
{
public:
    Scm(unsigned int first_cellsegs_) : last_cell_seg(-1), str_seglast(-1)
        {
            NIL = &NIL_; T = &T_; F = &F_; oblist = &NIL_;
            free_cell = &NIL_; fcells = 0;
            currentline = linebuff;
            endline = linebuff;

            for (size_t i = 0; OP_LET2AST >= i; ++i)
            { dispatch_table[i] = &Scm::opexe_0; }
            for (size_t i = OP_LET0REC; OP_CONTINUATION >= i; ++i)
            { dispatch_table[i] = &Scm::opexe_1; }
            for (size_t i = OP_ADD; OP_SETCDR >= i; ++i)
            { dispatch_table[i] = &Scm::opexe_2; }
            for (size_t i = OP_NOT; OP_EQV >= i; ++i)
            { dispatch_table[i] = &Scm::opexe_3; }
            for (size_t i = OP_FORCE; OP_NEWSEGMENT >= i; ++i)
            { dispatch_table[i] = &Scm::opexe_4; }
            for (size_t i = OP_RDSEXPR; OP_P1LIST >= i; ++i)
            { dispatch_table[i] = &Scm::opexe_5; }
            for (size_t i = OP_LIST_LENGTH; OP_CLOSUREP >= i; ++i)
            { dispatch_table[i] = &Scm::opexe_6; }
            dispatch_table[OP_MACROP] = &Scm::opexe_6;
            printf(banner);
            init_scheme(first_cellsegs_);
//             eval_file(InitFile);
        }
    virtual ~Scm(void) {}

    void eval_file(char *filename_)
        {
            long op = (long) OP_LOAD;
            args = cons(mk_string(filename_), NIL);
            Eval_Cycle(op);
        }

    pointer eval_str(char *func_)
        {
            args = cons(mk_string(func_), NIL);
//             Operator = (long) OP_T1LVL;

            if (strlen(func_) > BUFSIZ)
            { fprintf(stderr, "eval_str: strsize > BUFSIZ"); return NIL; }

            char buf[BUFSIZ];
            memcpy(buf, func_, BUFSIZ);
//             printf("buf: %s\n", buf);
            setbuf(infp, buf);

//             printf("eval_str ---\n");

            if (args->car()->isstring())
            {
                if (args->car()->strvalue() != NULL)
                { printf("str: \'%s\'", args->car()->strvalue()); }
            }

            // infp へ流し込む

            std::cerr << "need infp or filestream" << std::endl;

//             Eval_Cycle(OP_READ);
        }

protected:
    // allocate new cell segment
    int alloc_cellseg(int n)
        {
            register pointer p;
            register long i;
            register int k;

            for (k = 0; k < n; k++)
            {
                if (last_cell_seg >= CELL_NSEGMENT - 1) { return k; }
                p = (pointer) malloc(CELL_SEGSIZE * sizeof(class Cell));
                if (p == (pointer) 0) { return k; }
                cell_seg[++last_cell_seg] = p;
                fcells += CELL_SEGSIZE;
                for (i = 0; i < CELL_SEGSIZE - 1; i++, p++)
                { p->set_type(0); p->set_cdr(NIL); p->set_cdr(p + 1); }
                p->set_type(0);
                p->set_car(NIL);
                p->set_cdr(free_cell);
                free_cell = cell_seg[last_cell_seg];
            }
            return n;
        }

    // allocate new string segment
    int alloc_strseg(int n)
        {
            register char *p;
            register long i;
            register int k;

            for (k = 0; k < n; k++)
            {
                if (str_seglast >= STR_NSEGMENT) { return k; }
                p = (char *) malloc(STR_SEGSIZE * sizeof(char));
                if (p == (char *) 0) { return k; }
                str_seg[++str_seglast] = p;
                for (i = 0; i < STR_SEGSIZE; i++) { *p++ = (char) (-1); }
            }
            return n;
        }

    // ========== Error ==========
    void FatalError(char *fmt, char *a = NULL, char *b = NULL, char *c = NULL)
        {
            fprintf(stderr, "Fatal error: ");
            fprintf(stderr, fmt, a, b, c);
            fprintf(stderr, "\n");
            exit(1);
        }


    // initialization of Mini-Scheme
    void init_scheme(unsigned int first_cellsegs_)
        {
            if (alloc_cellseg(first_cellsegs_) != first_cellsegs_)
            { FatalError("Unable to allocate initial cell segments"); }
            if (!alloc_strseg(1))
            { FatalError("Unable to allocate initial string segments"); }
#ifdef VERBOSE
            gc_verbose = 1;
#else
            gc_verbose = 0;
#endif
            init_globals();
        }

    // get new cell.  parameter a, b is marked by gc.
    pointer get_cell(register pointer a, register pointer b)
        {
            register pointer x;

            if (free_cell == NIL)
            {
                gc(a, b);
                if (free_cell == NIL)
                if (!alloc_cellseg(1))
                { FatalError("run out of cells --- unable to recover cells"); }
            }
            x = free_cell;
            free_cell = x->cdr();
            --fcells;
            return x;
        }

    // get new cons cell
    pointer cons(register pointer a_, register pointer b_)
        {
            register pointer x = get_cell(a_, b_);

            x->set_type(T_PAIR);
            x->set_car(a_);
            x->set_cdr(b_);
            return x;
        }

    // get number atom
    pointer mk_number(register long num)
        {
            register pointer x = get_cell(NIL, NIL);

            x->set_type(T_NUMBER | T_ATOM);
            x->set_numvalue(num);
            return x;
        }

    // allocate name to string area
    char *store_string(char *name_)
        {
            register char *q;
            register long i;
            size_t len, remain;

            // first check name has already listed
            for (i = 0; i <= str_seglast; i++)
            {
                for (q = str_seg[i]; *q != (char) (-1); )
                {
                    if (!strcmp(q, name_)) { goto FOUND; }
                    while (*q++); // get next string
                }
            }
            len = strlen(name_) + 2;
            remain = STR_SEGSIZE - (q - str_seg[str_seglast]);
            if (remain < len)
            {
                if (!alloc_strseg(1)) { FatalError("run out of string area"); }
                q = str_seg[str_seglast];
            }
            strcpy(q, name_);
        FOUND:
            return q;
        }

    // get new string
    pointer mk_string(char *str_)
        {
            register pointer x = get_cell(NIL, NIL);

            x->set_strvalue(store_string(str_));
            x->set_type(T_STRING | T_ATOM);
            x->set_keynum((long) (-1));

            return x;
        }

    // get new symbol
    pointer mk_symbol(char *name_)
        {
            register pointer x;

            // first check oblist
            for (x = oblist; x != NIL; x = x->cdr())
            {
                if (!strcmp(name_, x->car()->symname())) { break; }
            }
            if (x != NIL) { return x->car(); }
            x = cons(mk_string(name_), NIL);
            x->set_type(T_SYMBOL);
            oblist = cons(x, oblist);
            return x;
        }

    // make symbol or number atom from string
    pointer mk_atom(char *q_)
        {
            char c, *p;

            p = q_;
            if (!isdigit(c = *p++))
            {
                if ((c != '+' && c != '-') || !isdigit(*p))
                { return mk_symbol(q_); }
            }
            for ( ; (c = *p) != 0; ++p)
            {
                if (!isdigit(c)) { return mk_symbol(q_); }
            }
            return mk_number(atol(q_));
        }

    // make constant
    pointer mk_const(char *name)
        {
            long x;
            char tmp[256];

            if (!strcmp(name, "t")) { return T; }
            if (!strcmp(name, "f")) { return F; }
//             if (*name == 'b')
//             {
//                 // #b (binary)
//                 sprintf(tmp, "b%s", &name[1]);
//                 sscanf(tmp, "%lx", &x);
//                 return (mk_number(x));
//             }
            if (*name == 'o')
            {
                // #o (octal)
                sprintf(tmp, "0%s", &name[1]);
                sscanf(tmp, "%lo", &x);
                return mk_number(x);
            }
            if (*name == 'd')
            {
                // #d (decimal)
                sscanf(&name[1], "%ld", &x);
                return mk_number(x);
            }
            if (*name == 'x')
            {
                // #x (hex)
                sprintf(tmp, "0x%s", &name[1]);
                sscanf(tmp, "%lx", &x);
                return mk_number(x);
            }
            return NIL;
        }


    // ========== garbage collector ==========
/*--
 *  We use algorithm E (Knuth, The Art of Computer Programming Vol.1,
 *  sec.3.5) for marking.
 */
    void mark(pointer a)
        {
            register pointer t, q, p;

        E1:
            t = (pointer) 0;
            p = a;
        E2:
            p->setmark();

        E3:
            if (p->isatom()) goto E6;
        E4:
            q = p->car();
            if (q && !q->ismark())
            {
                p->setatom();
                p->set_car(t);
                t = p;
                p = q;
                goto E2;
            }

        E5:
            q = p->cdr();
            if (q && !q->ismark())
            {
                p->set_cdr(t);
                t = p;
                p = q;
                goto E2;
            }
        E6:
            if (!t) { return; }
            q = t;
            if (q->isatom())
            {
                q->clratom();
                t = q->car();
                q->set_car(p);
                p = q;
                goto E5;
            }
            t = q->cdr();
            q->set_cdr(p);
            p = q;
            goto E6;
        }

    // garbage collection. parameter a, b is marked.
    void gc(register pointer a, register pointer b)
        {
            register pointer p;
            register long i;
            register long j;

            if (gc_verbose) { printf("gc..."); }

            // mark system globals
            mark(oblist);
            mark(global_env);

            // mark current registers
            mark(args);
            mark(envir);
            mark(code);
            mark(dump);

            // mark variables a, b
            mark(a);
            mark(b);

            // garbage collect
            NIL->clrmark();
            fcells = 0;
            free_cell = NIL;
            for (i = 0; i <= last_cell_seg; i++)
            {
                for (j = 0, p = cell_seg[i]; j < CELL_SEGSIZE; j++, p++)
                {
                    if (p->ismark()) { p->clrmark(); }
                    else
                    {
                        p->set_type(0);
                        p->set_cdr(free_cell);
                        p->set_car(NIL);
                        free_cell = p;
                        ++fcells;
                    }
                }
            }

            if (gc_verbose)
            { printf(" done %ld cells are recovered.\n", fcells); }
        }

// ========== Rootines for Reading ==========
#define TOK_LPAREN  0
#define TOK_RPAREN  1
#define TOK_DOT     2
#define TOK_ATOM    3
#define TOK_QUOTE   4
#define TOK_COMMENT 5
#define TOK_DQUOTE  6
#define TOK_BQUOTE  7
#define TOK_COMMA   8
#define TOK_ATMARK  9
#define TOK_SHARP   10

#define LINESIZE 1024

    // get new character from input file
    int inchar(void)
        {
            if (currentline >= endline)
            {	// input buffer is empty
                if (feof(infp))
                {
                    fclose(infp);
                    infp = stdin;
                    printf(prompt);
                }
                strcpy(linebuff, "\n");
                if (fgets(currentline = linebuff, LINESIZE, infp) == NULL)
                {
                    if (infp == stdin)
                    { fprintf(stderr, "Good-bye\n"); exit(0); }
                }
                endline = linebuff + strlen(linebuff);
            }
            return (*currentline++);
        }

    // clear input buffer
    void clearinput(void) { currentline = endline = linebuff; }

    // back to standard input
    void flushinput(void)
        {
            if (infp != stdin) { fclose(infp); infp = stdin; }
            clearinput();
        }

    // back character to input buffer
    void backchar(void) { currentline--; }

    // read chacters to delimiter
    char   *readstr(char *delim)
        {
            char *p = strbuff;

            while (isdelim(delim, (*p++ = inchar())));
            backchar();
            *--p = '\0';
            return strbuff;
        }

    // read string expression "xxx...xxx"
    char   *readstrexp(void)
        {
            char c, *p = strbuff;

            for (;;)
            {
                if ((c = inchar()) != '"') { *p++ = c; }
                else if (p > strbuff && *(p - 1) == '\\') { *(p - 1) = '"'; }
                else { *p = '\0'; return strbuff; }
            }
        }

    // check c is delimiter
    int isdelim(char *s, char c)
        {
            while (*s)
            {
                if (*s++ == c) { return 0; }
            }
            return 1;
        }

    // skip white characters
    void skipspace(void) { while (isspace(inchar())); backchar(); }

    // get token
    int token(void)
        {
            skipspace();

            switch (inchar())
            {
            case '(':
                return TOK_LPAREN;
            case ')':
                return TOK_RPAREN;
            case '.':
                return TOK_DOT;
            case '\'':
                return TOK_QUOTE;
            case ';':
                return TOK_COMMENT;
            case '"':
                return TOK_DQUOTE;
            case BACKQUOTE:
                return TOK_BQUOTE;
            case ',':
                if (inchar() == '@') { return TOK_ATMARK; }
                backchar();
                return TOK_COMMA;
            case '#':
                return TOK_SHARP;
            default:
                backchar();
                return TOK_ATOM;
            }
        }

// ========== Rootines for Printing ==========
#define	ok_abbrev(x)	((x)->ispair() && x->cdr() == NIL)

    void strunquote(char *p, char *s)
        {
            *p++ = '"';
            for ( ; *s; ++s)
            {
                if (*s == '"') { *p++ = '\\'; *p++ = '"'; }
                else if (*s == '\n') { *p++ = '\\'; *p++ = 'n'; }
                else { *p++ = *s; }
            }
            *p++ = '"';
            *p = '\0';
        }

    // print atoms
    int printatom(pointer l, int f)
        {
            char *p;

            if (l == NIL) { p = "()"; }
            else if (l == T) { p = "#t"; }
            else if (l == F) { p = "#f"; }
            else if (l->isnumber())
            {
                p = strbuff;
                sprintf(p, "%ld", l->numvalue());
            }
            else if (l->isstring())
            {
                if (!f) { p = l->strvalue(); }
                else { p = strbuff; strunquote(p, l->strvalue()); }
            }
            else if (l->issymbol()) { p = l->symname(); }
            else if (l->isproc())
            {
                p = strbuff;
                sprintf(p, "#<PROCEDURE %ld>", l->procnum());
            }
            else if (l->ismacro())
            {
                p = "#<MACRO>";
            }
            else if (l->isclosure()) { p = "#<CLOSURE>"; }
            else if (l->iscontinuation())	{ p = "#<CONTINUATION>"; }

            if (f < 0) { return static_cast<int>(strlen(p)); }
            fputs(p, outfp);
            return 0;
        }

    // ========== Rootines for Evaluation Cycle ==========
    // make closure. c is code. e is environment
    pointer mk_closure(register pointer c, register pointer e)
        {
            register pointer x = get_cell(c, e);

            x->set_type(T_CLOSURE);
            x->set_car(c);
            x->set_cdr(e);
            return x;
        }

    // make continuation.
    pointer mk_continuation(register pointer d)
        {
            register pointer x = get_cell(NIL, d);

            x->set_type(T_CONTINUATION);
            x->set_cont_dump(d);
            return x;
        }

    // reverse list -- make new cells
    pointer reverse(register pointer a) // a must be checked by gc
        {
            register pointer p = NIL;

            for (; a->ispair(); a = a->cdr()) { p = cons(a->car(), p); }
            return p;
        }

    // reverse list --- no make new cells
    pointer non_alloc_rev(pointer term, pointer list)
        {
            register pointer p = list, result = term, q;

            while (p != NIL)
            {
                q = p->cdr();
                p->set_cdr(result);
                result = p;
                p = q;
            }
            return result;
        }

    // append list -- make new cells
    pointer append(register pointer a, register pointer b)
        {
            register pointer p = b, q;

            if (a != NIL)
            {
                a = reverse(a);
                while (a != NIL)
                {
                    q = a->cdr();
                    a->set_cdr(p);
                    p = a;
                    a = q;
                }
            }
            return p;
        }

    // equivalence of atoms
    int eqv(register pointer a, register pointer b)
        {
            if (a->isstring())
            {
                if (b->isstring()) { return (a->strvalue() == b->strvalue()); }
                return 0;
            }
            if (a->isnumber())
            {
                if (b->isnumber()) { return (a->numvalue() == b->numvalue()); }
                return 0;
            }
            return (a == b);
        }

// Error
    pointer Error_0(char* str_)
        {
            args = cons(mk_string((str_)), NIL);
            Operator = (long)OP_ERR0;
            return T;
        }

    pointer Error_1(char* str_, pointer a_)
        {
            args = cons((a_), NIL);
            args = cons(mk_string((str_)), args);
            Operator = (long)OP_ERR0;
            return T;
        }

// for Eval_Cycle
    pointer s_goto(long a_)
        {
            Operator = (long)(a_);
            return T;
        }

    void s_save(long a_, pointer b_, pointer c_)
        {
            dump = cons(envir, cons((c_), dump));
            dump = cons((b_), dump);
            dump = cons(mk_number((long)(a_)), dump);
        }

    pointer s_return(pointer a_)
        {
            value = (a_);
            Operator = dump->car()->numvalue();
            args = dump->cadr();
            envir = dump->caddr();
            code = dump->cadddr();
            dump = dump->cddddr();
            return T;
        }

    pointer s_retbool(const bool b_) { return s_return(b_ ? T : F); }

    inline const bool istrue(pointer p_) { return p_ != NIL && p_ != F; }
    inline const bool isfalse(pointer p_) { return p_ == NIL || p_ == F; }

// ========== Evaluation Cycle ==========
    pointer opexe_0(register long op)
        {
            register pointer x, y;

            switch (op)
            {
            case OP_LOAD: // load
                if (!args->car()->isstring())
                { return Error_0("load -- argument is not string"); }
                if ((infp = fopen(args->car()->strvalue(), "r")) == NULL)
                {
                    infp = stdin;
                    return Error_1("Unable to open", args->car());
                }
                fprintf(outfp, "loading %s", args->car()->strvalue());
                return s_goto(OP_T0LVL);

            case OP_T0LVL: // top level
                fprintf(outfp, "\n");
                dump = NIL;
                envir = global_env;
                s_save(OP_VALUEPRINT, NIL, NIL);
                s_save(OP_T1LVL, NIL, NIL);
                if (infp == stdin) { printf(prompt); }
                return s_goto(OP_READ);

            case OP_T1LVL: // top level
                code = value;
                return s_goto(OP_EVAL);

            case OP_READ: // read
                tok = token();
                return s_goto(OP_RDSEXPR);

            case OP_VALUEPRINT:	// print evalution result
                print_flag = 1;
                args = value;
                s_save(OP_T0LVL, NIL, NIL);
                return s_goto(OP_P0LIST);

            case OP_EVAL: // main part of evalution
                if (code->issymbol())
                {
                    // symbol
                    for (x = envir; x != NIL; x = x->cdr())
                    {
                        for (y = x->car(); y != NIL; y = y->cdr())
                        {
                            if (y->caar() == code) { break; }
                        }
                        if (y != NIL) { break; }
                    }
                    if (x != NIL) { return s_return(y->cdar()); }
                     return Error_1("Unbounded variable", code);
                }
                if (code->ispair())
                {
                    x = code->car();
                    if (x->issyntax())
                    {	// SYNTAX
                        code = code->cdr();
                        return s_goto(x->syntaxnum());
                    }
                    // first, eval top element and eval arguments
                    s_save(OP_E0ARGS, NIL, code);
                    code = code->car();
                    return s_goto(OP_EVAL);
                }
                return s_return(code);

            case OP_E0ARGS: // eval arguments
                if (value->ismacro())
                { // macro expansion
                    s_save(OP_DOMACRO, NIL, NIL);
                    args = cons(code, NIL);
                    code = value;
                    return s_goto(OP_APPLY);
                }
                code = code->cdr();
                return s_goto(OP_E1ARGS);

            case OP_E1ARGS: // eval arguments
                args = cons(value, args);
                if (code->ispair())
                { // continue
                    s_save(OP_E1ARGS, args, code->cdr());
                    code = code->car();
                    args = NIL;
                    return s_goto(OP_EVAL);
                }
                // end
                args = reverse(args);
                code = args->car();
                args = args->cdr();
                return s_goto(OP_APPLY);

            case OP_APPLY: // apply 'code' to 'args'
                if (code->isproc())
                { return s_goto(code->procnum()); } // PROCEDURE
                if (code->isclosure()) // CLOSURE
                {
                    // make environment
                    envir = cons(NIL, code->closure_env());
                    for (x = code->closure_code()->car(), y = args;
                         x->ispair(); x = x->cdr(), y = y->cdr())
                    {
                        if (y == NIL) {  return Error_0("Few arguments"); }
                        envir->set_car(cons(cons(x->car(), y->car()),
                                            envir->car()));
                    }
                    if (x == NIL)
                    {
                        /*--
                         * if (y != NIL) {
                         * 	 return Error_0("Many arguments");
                         * }
                         */
                    }
                    else if (x->issymbol())
                    { envir->set_car(cons(cons(x, y), envir->car())); }
                    else {  return Error_0("Syntax error in closure"); }

                    code = code->closure_code()->cdr();
                    args = NIL;
                    return s_goto(OP_BEGIN);
                }
                if (code->iscontinuation()) // CONTINUATION
                {
                    dump = code->cont_dump();
                    return s_return(args != NIL ? args->car() : NIL);
                }
                 return Error_0("Illegal function");

            case OP_DOMACRO: // do macro
                code = value;
                return s_goto(OP_EVAL);

            case OP_LAMBDA: // lambda
                return s_return(mk_closure(code, envir));

            case OP_QUOTE: // quote
                return s_return(code->car());

            case OP_DEF0: // define
                if (code->car()->ispair())
                {
                    x = code->caar();
                    code = cons(LAMBDA, cons(code->cdar(), code->cdr()));
                }
                else { x = code->car(); code = code->cadr(); }
                if (!x->issymbol())
                { return Error_0("Variable is not symbol"); }
                s_save(OP_DEF1, NIL, x);
                return s_goto(OP_EVAL);

            case OP_DEF1: // define
                for (x = envir->car(); x != NIL; x = x->cdr())
                {
                    if (x->caar() == code) { break; }
                }
                if (x != NIL) { x->set_cdar(value); }
                else { envir->set_car(cons(cons(code, value), envir->car())); }
                return s_return(code);

            case OP_SET0: // set!
                s_save(OP_SET1, NIL, code->car());
                code = code->cadr();
                return s_goto(OP_EVAL);

            case OP_SET1: // set!
                for (x = envir; x != NIL; x = x->cdr())
                {
                    for (y = x->car(); y != NIL; y = y->cdr())
                    {
                        if (y->caar() == code) { break; }
                    }
                    if (y != NIL) { break; }
                }
                if (x != NIL) { y->set_cdar(value); return s_return(value); }
                 return Error_1("Unbounded variable", code);

            case OP_BEGIN: // begin
                if (!code->ispair()) { return s_return(code); }
                if (code->cdr() != NIL) { s_save(OP_BEGIN, NIL, code->cdr()); }
                code = code->car();
                return s_goto(OP_EVAL);

            case OP_IF0: // if
                s_save(OP_IF1, NIL, code->cdr());
                code = code->car();
                return s_goto(OP_EVAL);

            case OP_IF1: // if
                if (istrue(value)) { code = code->car(); }
                else { code = code->cadr(); } /* (if #f 1) ==> () because
                                               * NIL->set_car(NIL) */
                return s_goto(OP_EVAL);

            case OP_LET0: // let
                args = NIL;
                value = code;
                code = code->car()->issymbol() ? code->cadr() : code->car();
                return s_goto(OP_LET1);

            case OP_LET1: // let (caluculate parameters)
                args = cons(value, args);
                if (code->ispair()) // continue
                {
                    s_save(OP_LET1, args, code->cdr());
                    code = code->cadar();
                    args = NIL;
                    return s_goto(OP_EVAL);
                }
                // end
                args = reverse(args);
                code = args->car();
                args = args->cdr();
                return s_goto(OP_LET2);

            case OP_LET2: // let
                envir = cons(NIL, envir);
                for (x = code->car()->issymbol() ? code->cadr() : code->car(),
                         y = args; y != NIL; x = x->cdr(), y = y->cdr())
                {
                    envir->set_car(cons(cons(x->caar(),
                                             y->car()), envir->car()));
                }
                if (code->car()->issymbol())
                { // named let
                    for (x = code->cadr(), args = NIL; x != NIL; x = x->cdr())
                    { args = cons(x->caar(), args); }
                    x = mk_closure(cons(reverse(args), code->cddr()), envir);
                    envir->set_car(cons(cons(code->car(), x), envir->car()));
                    code = code->cddr();
                    args = NIL;
                }
                else { code = code->cdr(); args = NIL; }
                return s_goto(OP_BEGIN);

            case OP_LET0AST: // let*
                if (code->car() == NIL)
                {
                    envir = cons(NIL, envir);
                    code = code->cdr();
                    return s_goto(OP_BEGIN);
                }
                s_save(OP_LET1AST, code->cdr(), code->car());
                code = code->cadaar();
                return s_goto(OP_EVAL);

            case OP_LET1AST: // let* (make new frame)
                envir = cons(NIL, envir);
                return s_goto(OP_LET2AST);

            case OP_LET2AST: // let* (caluculate parameters)
                envir->set_car(cons(cons(code->caar(), value), envir->car()));
                code = code->cdr();
                if (code->ispair()) // continue
                {
                    s_save(OP_LET2AST, args, code);
                    code = code->cadar();
                    args = NIL;
                    return s_goto(OP_EVAL);
                }
                // end
                code = args;
                args = NIL;
                return s_goto(OP_BEGIN);

            default:
                sprintf(strbuff, "%d is illegal operator", Operator);
                return Error_0(strbuff);
            }
            return T;
        }

    pointer opexe_1(register long op)
        {
            register pointer x, y;

            switch (op)
            {
            case OP_LET0REC: // letrec
                envir = cons(NIL, envir);
                args = NIL;
                value = code;
                code = code->car();
                return s_goto(OP_LET1REC);

            case OP_LET1REC: // letrec (caluculate parameters)
                args = cons(value, args);
                if (code->ispair()) // continue
                {
                    s_save(OP_LET1REC, args, code->cdr());
                    code = code->cadar();
                    args = NIL;
                    return s_goto(OP_EVAL);
                }
                // end
                args = reverse(args);
                code = args->car();
                args = args->cdr();
                return s_goto(OP_LET2REC);

            case OP_LET2REC: // letrec
                for (x = code->car(), y = args; y != NIL;
                     x = x->cdr(), y = y->cdr())
                {
                    envir->set_car(cons(cons(x->caar(),
                                             y->car()), envir->car()));
                }
                code = code->cdr();
                args = NIL;
                return s_goto(OP_BEGIN);

            case OP_COND0: // cond
                if (!code->ispair())
                { return Error_0("Syntax error in cond"); }
                s_save(OP_COND1, NIL, code);
                code = code->caar();
                return s_goto(OP_EVAL);

            case OP_COND1: // cond
                if (istrue(value))
                {
                    if ((code = code->cdar()) == NIL)
                    { return s_return(value); }
                    return s_goto(OP_BEGIN);
                }
                if ((code = code->cdr()) == NIL) { return s_return(NIL); }

                s_save(OP_COND1, NIL, code);
                code = code->caar();
                return s_goto(OP_EVAL);

            case OP_DELAY: // delay
                x = mk_closure(cons(NIL, code), envir);
                x->setpromise();
                return s_return(x);

            case OP_AND0: // and
                if (code == NIL) { return s_return(T); }
                s_save(OP_AND1, NIL, code->cdr());
                code = code->car();
                return s_goto(OP_EVAL);

            case OP_AND1: // and
                if (isfalse(value)) { return s_return(value); }
                if (code == NIL) { return s_return(value); }
                s_save(OP_AND1, NIL, code->cdr());
                code = code->car();
                return s_goto(OP_EVAL);

            case OP_OR0: // or
                if (code == NIL) { return s_return(F); }
                s_save(OP_OR1, NIL, code->cdr());
                code = code->car();
                return s_goto(OP_EVAL);

            case OP_OR1: // or
                if (istrue(value)) { return s_return(value); }
                if (code == NIL) { return s_return(value); }
                s_save(OP_OR1, NIL, code->cdr());
                code = code->car();
                return s_goto(OP_EVAL);

            case OP_C0STREAM: // cons-stream
                s_save(OP_C1STREAM, NIL, code->cdr());
                code = code->car();
                return s_goto(OP_EVAL);

            case OP_C1STREAM: // cons-stream
                args = value; // save value to register args for gc
                x = mk_closure(cons(NIL, code), envir);
                x->setpromise();
                return s_return(cons(args, x));

            case OP_0MACRO: // macro
                x = code->car();
                code = code->cadr();
                if (!x->issymbol())
                { return Error_0("Variable is not symbol"); }
                s_save(OP_1MACRO, NIL, x);
                return s_goto(OP_EVAL);

            case OP_1MACRO: // macro
                value->set_type(value->type() | T_MACRO);
                for (x = envir->car(); x != NIL; x = x->cdr())
                {
                    if (x->caar() == code) { break; }
                }
                if (x != NIL) { x->set_cdar(value); }
                else { envir->set_car(cons(cons(code, value), envir->car())); }
                return s_return(code);

            case OP_CASE0: // case
                s_save(OP_CASE1, NIL, code->cdr());
                code = code->car();
                return s_goto(OP_EVAL);

            case OP_CASE1: // case
                for (x = code; x != NIL; x = x->cdr())
                {
                    y = x->caar();
                    if (!y->ispair()) { break; }
                    for ( ; y != NIL; y = y->cdr())
                    {
                        if (eqv(y->car(), value)) { break; }
                    }
                    if (y != NIL) { break; }
                }
                if (x != NIL)
                {
                    if (x->caar()->ispair())
                    { code = x->cdar(); return s_goto(OP_BEGIN); }
                    s_save(OP_CASE2, NIL, x->cdar());
                    code = x->caar();
                    return s_goto(OP_EVAL);
                }
                return s_return(NIL);

            case OP_CASE2: // case
                if (istrue(value)) { return s_goto(OP_BEGIN); }
                return s_return(NIL);

            case OP_PAPPLY: // apply
                code = args->car();
                args = args->cadr();
                return s_goto(OP_APPLY);

            case OP_PEVAL: // eval
                code = args->car();
                args = NIL;
                return s_goto(OP_EVAL);

            case OP_CONTINUATION: // call-with-current-continuation
                code = args->car();
                args = cons(mk_continuation(dump), NIL);
                return s_goto(OP_APPLY);

            default:
                sprintf(strbuff, "%d is illegal operator", Operator);
                 return Error_0(strbuff);
            }
            return T;
        }

    pointer opexe_2(register long op)
        {
            register pointer x;
            register long v;

            switch (op)
            {
            case OP_ADD: // +
                for (x = args, v = 0; x != NIL; x = x->cdr())
                { v += x->car()->numvalue(); }
                return s_return(mk_number(v));

            case OP_SUB: // -
                for (x = args->cdr(), v = args->car()->numvalue();
                     x != NIL; x = x->cdr())
                { v -= x->car()->numvalue(); }
                return s_return(mk_number(v));

            case OP_MUL: // *
                for (x = args, v = 1; x != NIL; x = x->cdr())
                { v *= x->car()->numvalue(); }
                return s_return(mk_number(v));

            case OP_DIV: // /
                for (x = args->cdr(), v = args->car()->numvalue();
                     x != NIL; x = x->cdr())
                {
                    if (x->car()->numvalue() != 0)
                    { v /= x->car()->numvalue(); }
                    else {  return Error_0("Divided by zero"); }
                }
                return s_return(mk_number(v));

            case OP_REM: // remainder
                for (x = args->cdr(), v = args->car()->numvalue();
                     x != NIL; x = x->cdr())
                {
                    if (x->car()->numvalue() != 0)
                    { v %= x->car()->numvalue(); }
                    else {  return Error_0("Divided by zero"); }
                }
                return s_return(mk_number(v));

            case OP_CAR: // car
                if (args->car()->ispair()) { return s_return(args->caar()); }
                 return Error_0("Unable to car for non-cons cell");

            case OP_CDR: // cdr
                if (args->car()->ispair()) { return s_return(args->cdar()); }
                 return Error_0("Unable to cdr for non-cons cell");

            case OP_CONS: // cons
                args->set_cdr(args->cadr());
                return s_return(args);

            case OP_SETCAR: // set-car!
                if (args->car()->ispair())
                { args->set_car(args->cadr()); return s_return(args->car()); }
                 return Error_0("Unable to set-car! for non-cons cell");

            case OP_SETCDR: // set-cdr!
                if (args->car()->ispair())
                { args->set_car(args->cadr()); return s_return(args->car()); }
                 return Error_0("Unable to set-cdr! for non-cons cell");

            default:
                sprintf(strbuff, "%d is illegal operator", Operator);
                 return Error_0(strbuff);
            }
            return T;
        }


    pointer opexe_3(register long op)
        {
            switch (op)
            {
            case OP_NOT: return s_retbool(isfalse(args->car()));// not
            case OP_BOOL: return s_retbool(args->car() == F
                                           || args->car() == T);// bool?
            case OP_NULL: return s_retbool(args->car() == NIL);// null?
            case OP_ZEROP: return s_retbool(args->car()->numvalue()
                                            == 0);// zero?
            case OP_POSP: return s_retbool(args->car()->numvalue()
                                           > 0);// positive?
            case OP_NEGP: return s_retbool(args->car()->numvalue()
                                           < 0);// negative?
            case OP_NEQ: return s_retbool(args->car()->numvalue()
                                          == args->cadr()->numvalue());// =
            case OP_LESS: return s_retbool(args->car()->numvalue()
                                           < args->cadr()->numvalue());// <
            case OP_GRE: return s_retbool(args->car()->numvalue()
                                          > args->cadr()->numvalue());// >
            case OP_LEQ: return s_retbool(args->car()->numvalue()
                                          <= args->cadr()->numvalue());//<=
            case OP_GEQ: return s_retbool(args->car()->numvalue()
                                          >= args->cadr()->numvalue());//=>
            case OP_SYMBOL:
                return s_retbool(args->car()->issymbol());// symbol?
            case OP_NUMBER:
                return s_retbool(args->car()->isnumber());// number?
            case OP_STRING:
                return s_retbool(args->car()->isstring());// string?
            case OP_PROC: // procedure?
                /*--
                 * continuation should be procedure by the example
                 * (call-with-current-continuation procedure?) ==> #t
                 * in R^3 report sec. 6.9
                 */
                return s_retbool(args->car()->isproc()
                                 || args->car()->isclosure()
                          || args->car()->iscontinuation());
            case OP_PAIR: return s_retbool(args->car()->ispair());// pair?
            case OP_EQ: return s_retbool(args->car() == args->cadr());// eq?
            case OP_EQV:
                return s_retbool(eqv(args->car(), args->cadr()));// eqv?
            default:
                sprintf(strbuff, "%d is illegal operator", Operator);
                return Error_0(strbuff);
            }
            return T;
        }

    pointer opexe_4(register long op)
        {
            register pointer x, y;

            switch (op)
            {
            case OP_FORCE: // force
                code = args->car();
                if (code->ispromise()) { args = NIL; return s_goto(OP_APPLY); }
                return s_return(code);

            case OP_WRITE: // write
                print_flag = 1;
                args = args->car();
                return s_goto(OP_P0LIST);

            case OP_DISPLAY: // display
                print_flag = 0;
                args = args->car();
                return s_goto(OP_P0LIST);

            case OP_NEWLINE: // newline
                fprintf(outfp, "\n");
                return s_return(T);

            case OP_ERR0: // error
                if (!(args->car()->isstring()))
                {  return Error_0("error -- first argument must be string"); }
                tmpfp = outfp;
                outfp = stderr;
                fprintf(outfp, "Error: ");
                fprintf(outfp, "%s", args->car()->strvalue());
                args = args->cdr();
                return s_goto(OP_ERR1);

            case OP_ERR1: // error
                fprintf(outfp, " ");
                if (args != NIL)
                {
                    s_save(OP_ERR1, args->cdr(), NIL);
                    args = args->car();
                    print_flag = 1;
                    return s_goto(OP_P0LIST);
                }
                fprintf(outfp, "\n");
                flushinput();
                outfp = tmpfp;
                return s_goto(OP_T0LVL);

            case OP_REVERSE: // reverse
                return s_return(reverse(args->car()));

            case OP_APPEND: // append
                return s_return(append(args->car(), args->cadr()));

            case OP_PUT: // put
                if (!(args->car()->hasprop()) || !(args->cadr()->hasprop()))
                {  return Error_0("Illegal use of put"); }
                for (x = args->car()->symprop(), y = args->cadr(); x != NIL;
                     x = x->cdr())
                {
                    if (x->caar() == y) { break; }
                }
                if (x != NIL) { x->set_cdar(args->caddr()); }
                else
                {
                    args->car()->set_symprop(cons(cons(y, args->caddr()),
                                                  args->car()->symprop()));
                }
                return s_return(T);

            case OP_GET: // get
                if (!(args->car()->hasprop()) || !(args->cadr()->hasprop()))
                {  return Error_0("Illegal use of get"); }
                for (x = args->car()->symprop(), y = args->cadr(); x != NIL;
                     x = x->cdr())
                {
                    if (x->caar() == y) { break; }
                }
                if (x != NIL) { return s_return(x->cdar()); }
                return s_return(NIL);

            case OP_QUIT: // quit
                return NIL;

            case OP_GC: // gc
                gc(NIL, NIL);
                return s_return(T);

            case OP_GCVERB: // gc-verbose
                {
                    int was = gc_verbose;
                    gc_verbose = (args->car() != F);
                    return s_retbool(was);
                }
            case OP_NEWSEGMENT:	// new-segment
                if (!(args->car()->isnumber()))
                {  return Error_0("new-segment -- argument must be number"); }
                fprintf(outfp, "allocate %d new segments\n",
                        alloc_cellseg((int) args->car()->numvalue()));
                return s_return(T);
            }

            // not reached
            fprintf(stderr, "Something wrong: opexe_4");
            return NIL;
        }

    pointer opexe_5(register long op)
        {
            register pointer x;

            switch (op)
            {
                // ========== reading part ==========
                case OP_RDSEXPR:
                switch (tok)
                {
                case TOK_COMMENT:
                    while (inchar() != '\n');
                    tok = token();
                    return s_goto(OP_RDSEXPR);
                case TOK_LPAREN:
                    tok = token();
                    if (tok == TOK_RPAREN) { return s_return(NIL); }
                    if (tok == TOK_DOT)
                    { return Error_0("syntax error--illegal dot expression"); }
                    s_save(OP_RDLIST, NIL, NIL);
                    return s_goto(OP_RDSEXPR);
                case TOK_QUOTE:
                    s_save(OP_RDQUOTE, NIL, NIL);
                    tok = token();
                    return s_goto(OP_RDSEXPR);
                case TOK_BQUOTE:
                    s_save(OP_RDQQUOTE, NIL, NIL);
                    tok = token();
                    return s_goto(OP_RDSEXPR);
                case TOK_COMMA:
                    s_save(OP_RDUNQUOTE, NIL, NIL);
                    tok = token();
                    return s_goto(OP_RDSEXPR);
                case TOK_ATMARK:
                    s_save(OP_RDUQTSP, NIL, NIL);
                    tok = token();
                    return s_goto(OP_RDSEXPR);
                case TOK_ATOM:
                    return s_return(mk_atom(readstr("();\t\n ")));
                case TOK_DQUOTE:
                    return s_return(mk_string(readstrexp()));
                case TOK_SHARP:
                    if ((x = mk_const(readstr("();\t\n "))) == NIL)
                    {  return Error_0("Undefined sharp expression"); }
                    return s_return(x);
                default:
                     return Error_0("syntax error -- illegal token");
                }
                break;

            case OP_RDLIST:
                args = cons(value, args);
                tok = token();
                if (tok == TOK_COMMENT)
                {
                    while (inchar() != '\n');
                    tok = token();
                }
                if (tok == TOK_RPAREN)
                { return s_return(non_alloc_rev(NIL, args)); }
                if (tok == TOK_DOT)
                {
                    s_save(OP_RDDOT, args, NIL);
                    tok = token();
                    return s_goto(OP_RDSEXPR);
                }
                s_save(OP_RDLIST, args, NIL);
                return s_goto(OP_RDSEXPR);

            case OP_RDDOT:
                if (token() != TOK_RPAREN)
                {  return Error_0("syntax error -- illegal dot expression"); }
                return s_return(non_alloc_rev(value, args));

            case OP_RDQUOTE:
                return s_return(cons(QUOTE, cons(value, NIL)));

            case OP_RDQQUOTE:
                return s_return(cons(QQUOTE, cons(value, NIL)));

            case OP_RDUNQUOTE:
                return s_return(cons(UNQUOTE, cons(value, NIL)));

            case OP_RDUQTSP:
                return s_return(cons(UNQUOTESP, cons(value, NIL)));

            // ========== printing part ==========
            case OP_P0LIST:
                if (!(args->ispair()))
                {
                    printatom(args, print_flag);
                    return s_return(T);
                }
                if (args->car() == QUOTE && ok_abbrev(args->cdr()))
                {
                    fprintf(outfp, "'");
                    args = args->cadr();
                    return s_goto(OP_P0LIST);
                }
                if (args->car() == QQUOTE && ok_abbrev(args->cdr()))
                {
                    fprintf(outfp, "`");
                    args = args->cadr();
                    return s_goto(OP_P0LIST);
                }
                if (args->car() == UNQUOTE && ok_abbrev(args->cdr()))
                {
                    fprintf(outfp, ",");
                    args = args->cadr();
                    return s_goto(OP_P0LIST);
                }
                if (args->car() == UNQUOTESP && ok_abbrev(args->cdr()))
                {
                    fprintf(outfp, ",@");
                    args = args->cadr();
                    return s_goto(OP_P0LIST);
                }
                fprintf(outfp, "(");
                s_save(OP_P1LIST, args->cdr(), NIL);
                args = args->car();
                return s_goto(OP_P0LIST);

            case OP_P1LIST:
                if (args->ispair())
                {
                    s_save(OP_P1LIST, args->cdr(), NIL);
                    fprintf(outfp, " ");
                    args = args->car();
                    return s_goto(OP_P0LIST);
                }
                if (args != NIL)
                { fprintf(outfp, " . "); printatom(args, print_flag); }
                fprintf(outfp, ")");
                return s_return(T);

            default:
                sprintf(strbuff, "%d is illegal operator", Operator);
                 return Error_0(strbuff);
            }
            return T;
        }

    pointer opexe_6(register long op)
        {
            register pointer x, y;
            register long v;
            static long	w;

            switch (op)
            {
            case OP_LIST_LENGTH: // list-length /* a.k */
                for (x = args->car(), v = 0; x->ispair(); x = x->cdr())
                { ++v; }
                return s_return(mk_number(v));

            case OP_ASSQ: // assq /* a.k */
                x = args->car();
                for (y = args->cadr(); y->ispair(); y = y->cdr())
                {
                    if (!(y->car()->ispair()))
                    {  return Error_0("Unable to handle non pair element"); }
                    if (x == y->caar()) { break; }
                }
                if (y->ispair()) { return s_return(y->car()); }
                return s_return(F);

            case OP_PRINT_WIDTH: // print-width /* a.k */
                w = 0;
                args = args->car();
                print_flag = -1;
                return s_goto(OP_P0_WIDTH);

            case OP_P0_WIDTH:
                if (!args->ispair())
                {
                    w += printatom(args, print_flag);
                    return s_return(mk_number(w));
                }
                if (args->car() == QUOTE && ok_abbrev(args->cdr()))
                {
                    ++w;
                    args = args->cadr();
                    return s_goto(OP_P0_WIDTH);
                }
                if (args->car() == QQUOTE && ok_abbrev(args->cdr()))
                {
                    ++w;
                    args = args->cadr();
                    return s_goto(OP_P0_WIDTH);
                }
                if (args->car() == UNQUOTE && ok_abbrev(args->cdr()))
                {
                    ++w;
                    args = args->cadr();
                    return s_goto(OP_P0_WIDTH);
                }
                if (args->car() == UNQUOTESP && ok_abbrev(args->cdr()))
                {
                    w += 2;
                    args = args->cadr();
                    return s_goto(OP_P0_WIDTH);
                }
                ++w;
                s_save(OP_P1_WIDTH, args->cdr(), NIL);
                args = args->car();
                return s_goto(OP_P0_WIDTH);

            case OP_P1_WIDTH:
                if (args->ispair())
                {
                    s_save(OP_P1_WIDTH, args->cdr(), NIL);
                    ++w;
                    args = args->car();
                    return s_goto(OP_P0_WIDTH);
                }
                if (args != NIL) { w += 3 + printatom(args, print_flag); }
                ++w;
                return s_return(mk_number(w));

            case OP_GET_CLOSURE: // get-closure-code /* a.k */
                args = args->car();
                if (args == NIL) { return s_return(F); }
                if (args->isclosure())
                { return s_return(cons(LAMBDA, value->closure_code())); }
                if (args->ismacro())
                { return s_return(cons(LAMBDA, value->closure_code())); }
                return s_return(F);

            case OP_CLOSUREP: // closure?
                /*
                 * Note, macro object is also a closure.
                 * Therefore, (closure? <#MACRO>) ==> #t
                 */
                if (args->car() == NIL) { return s_return(F); }
                return s_retbool(args->car()->isclosure());
            case OP_MACROP: // macro?
                if (args->car() == NIL) { return s_return(F); }
                return s_retbool(args->car()->ismacro());
            default:
                sprintf(strbuff, "%d is illegal operator", Operator);
                 return Error_0(strbuff);
            }
            return T;	// NOTREACHED
        }

    // kernel of this intepreter
    pointer Eval_Cycle(register long op)
        {
            Operator = op;
            for (;;)
            {
                if ((this->*dispatch_table[Operator])(Operator) == NIL)
                { return NIL; }
            }
        }

    // ========== Initialization of internal keywords ==========
    void mk_syntax(short op, char *name)
        {
            pointer x;

            x = cons(mk_string(name), NIL);
            x->set_type(T_SYNTAX | T_SYMBOL);
            x->set_syntaxnum(op);
            oblist = cons(x, oblist);
        }

    void mk_proc(unsigned long op, char *name)
        {
            pointer x, y;

            x = mk_symbol(name);
            y = get_cell(NIL, NIL);
            y->set_type(T_PROC | T_ATOM);
            y->set_numvalue((long) op);
            global_env->set_car(cons(cons(x, y), global_env->car()));
        }

    void init_vars_global(void)
        {
            pointer x;

            // init input/output file
            infp = stdin;
            outfp = stdout;
            // init NIL
            NIL->set_type(T_ATOM | MARK);
            NIL->set_car(NIL); NIL->set_cdr(NIL);
            // init T
            T->set_type(T_ATOM | MARK);
            T->set_car(T); T->set_cdr(T);
            // init F
            F->set_type(T_ATOM | MARK);
            F->set_car(F); F->set_cdr(F);
            // init global_env
            global_env = cons(NIL, NIL);
            // init else
            x = mk_symbol("else");
            global_env->set_car(cons(cons(x, T), global_env->car()));
        }

    void init_syntax(void)
        {
            // init syntax
            mk_syntax(OP_LAMBDA, "lambda");
            mk_syntax(OP_QUOTE, "quote");
            mk_syntax(OP_DEF0, "define");
            mk_syntax(OP_IF0, "if");
            mk_syntax(OP_BEGIN, "begin");
            mk_syntax(OP_SET0, "set!");
            mk_syntax(OP_LET0, "let");
            mk_syntax(OP_LET0AST, "let*");
            mk_syntax(OP_LET0REC, "letrec");
            mk_syntax(OP_COND0, "cond");
            mk_syntax(OP_DELAY, "delay");
            mk_syntax(OP_AND0, "and");
            mk_syntax(OP_OR0, "or");
            mk_syntax(OP_C0STREAM, "cons-stream");
            mk_syntax(OP_0MACRO, "macro");
            mk_syntax(OP_CASE0, "case");
        }

    void init_procs(void)
        {
            // init procedure
            mk_proc(OP_PEVAL, "eval");
            mk_proc(OP_PAPPLY, "apply");
            mk_proc(OP_CONTINUATION, "call-with-current-continuation");
            mk_proc(OP_FORCE, "force");
            mk_proc(OP_CAR, "car");
            mk_proc(OP_CDR, "cdr");
            mk_proc(OP_CONS, "cons");
            mk_proc(OP_SETCAR, "set-car!");
            mk_proc(OP_SETCDR, "set-cdr!");
            mk_proc(OP_ADD, "+");
            mk_proc(OP_SUB, "-");
            mk_proc(OP_MUL, "*");
            mk_proc(OP_DIV, "/");
            mk_proc(OP_REM, "remainder");
            mk_proc(OP_NOT, "not");
            mk_proc(OP_BOOL, "boolean?");
            mk_proc(OP_SYMBOL, "symbol?");
            mk_proc(OP_NUMBER, "number?");
            mk_proc(OP_STRING, "string?");
            mk_proc(OP_PROC, "procedure?");
            mk_proc(OP_PAIR, "pair?");
            mk_proc(OP_EQV, "eqv?");
            mk_proc(OP_EQ, "eq?");
            mk_proc(OP_NULL, "null?");
            mk_proc(OP_ZEROP, "zero?");
            mk_proc(OP_POSP, "positive?");
            mk_proc(OP_NEGP, "negative?");
            mk_proc(OP_NEQ, "=");
            mk_proc(OP_LESS, "<");
            mk_proc(OP_GRE, ">");
            mk_proc(OP_LEQ, "<=");
            mk_proc(OP_GEQ, ">=");
            mk_proc(OP_READ, "read");
            mk_proc(OP_WRITE, "write");
            mk_proc(OP_DISPLAY, "display");
            mk_proc(OP_NEWLINE, "newline");
            mk_proc(OP_LOAD, "load");
            mk_proc(OP_ERR0, "error");
            mk_proc(OP_REVERSE, "reverse");
            mk_proc(OP_APPEND, "append");
            mk_proc(OP_PUT, "put");
            mk_proc(OP_GET, "get");
            mk_proc(OP_GC, "gc");
            mk_proc(OP_GCVERB, "gc-verbose");
            mk_proc(OP_NEWSEGMENT, "new-segment");
            mk_proc(OP_LIST_LENGTH, "list-length");
            mk_proc(OP_ASSQ, "assq");
            mk_proc(OP_PRINT_WIDTH, "print-width");
            mk_proc(OP_GET_CLOSURE, "get-closure-code");
            mk_proc(OP_CLOSUREP, "closure?");
            mk_proc(OP_MACROP, "macro?");
            mk_proc(OP_QUIT, "quit");
        }

    // initialize several globals
    void init_globals(void)
        {
            init_vars_global();
            init_syntax();
            init_procs();
            // intialization of global pointers to special symbols
            LAMBDA = mk_symbol("lambda");
            QUOTE = mk_symbol("quote");
            QQUOTE = mk_symbol("quasiquote");
            UNQUOTE = mk_symbol("unquote");
            UNQUOTESP = mk_symbol("unquote-splicing");
        }

private:
    // arrays for segments
    pointer cell_seg[CELL_NSEGMENT];
    int     last_cell_seg;
    char   *str_seg[STR_NSEGMENT];
    int     str_seglast;

    // We use 4 registers.
    pointer args;       // register for arguments of function
    pointer envir;      // stack register for current environment
    pointer code;       // register for current code
    pointer dump;       // stack register for next evaluation

    Cell NIL_;
    pointer NIL;        // special cell representing empty cell
    Cell T_;
    pointer T;          // special cell representing #t
    Cell F_;
    pointer F;          // special cell representing #f
    pointer oblist;     // pointer to symbol table
    pointer global_env; // pointer to global environment

    // global pointers to special symbols
    pointer LAMBDA;     // pointer to syntax lambda
    pointer QUOTE;      // pointer to syntax quote

    pointer QQUOTE;     // pointer to symbol quasiquote
    pointer UNQUOTE;    // pointer to symbol unquote
    pointer UNQUOTESP;  // pointer to symbol unquote-splicing

    pointer (Scm<NumT, StrT>::*dispatch_table[OP_MACROP + 1])(long);

    pointer free_cell;  // pointer to top of free cells
    long    fcells;     // # of free cells

    FILE   *infp;       // input file
    FILE   *outfp;      // output file

    char    gc_verbose; // if gc_verbose is not zero, print gc status

    char    linebuff[LINESIZE];
    char    strbuff[256];
    char   *currentline;
    char   *endline;

    FILE *tmpfp;
    int tok;
    int print_flag;
    pointer value;
    long Operator;
};
}
