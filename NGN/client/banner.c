/*	from Unix 6th Edition /usr/source/s1/banner.c	*/
/*
 * Copyright(C) Caldera International Inc. 2001-2002. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   Redistributions of source code and documentation must retain the
 *    above copyright notice, this list of conditions and the following
 *    disclaimer.
 *   Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *   All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed or owned by Caldera
 *      International, Inc.
 *   Neither the name of Caldera International, Inc. nor the names of
 *    other contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * USE OF THE SOFTWARE PROVIDED FOR UNDER THIS LICENSE BY CALDERA
 * INTERNATIONAL, INC. AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL CALDERA INTERNATIONAL, INC. BE
 * LIABLE FOR ANY DIRECT, INDIRECT INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if __GNUC__ >= 3 && __GNUC_MINOR__ >= 4 || __GNUC__ >= 4
#define	USED	__attribute__ ((used))
#elif defined __GNUC__
#define	USED	__attribute__ ((unused))
#else
#define	USED
#endif
static const char sccsid[] USED = "@(#)banner.sl	1.5 (gritter) 5/29/05";

#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>

#define nchars 256	/*number of chars in char set*/
#define nlines  7	/*number of lines in a banner character*/
#define pposs  85	/*number of print positions on a line (must be multiple of 4)*/
			/*followed by end of string character*/
#define pospch 8	/*number of char positions per banner char*/
#define chpln  10	/*number of banner characters per line*/

struct bann{
	  char alpha[nlines][pposs];
};
struct bann buffer,*bp=&buffer;
const char ctbl[nchars][nlines]={
	{ 0,0,0,0,0,0,0 },			/* \0 */
	{ 0,0,0,0,0,0,0 },			/* \1 */
	{ 0,0,0,0,0,0,0 },			/* \2 */
	{ 0,0,0,0,0,0,0 },			/* \3 */
	{ 0,0,0,0,0,0,0 },			/* \4 */
	{ 0,0,0,0,0,0,0 },			/* \5 */
	{ 0,0,0,0,0,0,0 },			/* \6 */
	{ 0,0,0,0,0,0,0 },			/* \7 */
	{ 0,0,0,0,0,0,0 },			/* \10 */
	{ 0,0,0,0,0,0,0 },			/* \11 */
	{ 0,0,0,0,0,0,0 },			/* \12 */
	{ 0,0,0,0,0,0,0 },			/* \13 */
	{ 0,0,0,0,0,0,0 },			/* \14 */
	{ 0,0,0,0,0,0,0 },			/* \15 */
	{ 0,0,0,0,0,0,0 },			/* \16 */
	{ 0,0,0,0,0,0,0 },			/* \17 */
	{ 0,0,0,0,0,0,0 },			/* \20 */
	{ 0,0,0,0,0,0,0 },			/* \21 */
	{ 0,0,0,0,0,0,0 },			/* \22 */
	{ 0,0,0,0,0,0,0 },			/* \23 */
	{ 0,0,0,0,0,0,0 },			/* \24 */
	{ 0,0,0,0,0,0,0 },			/* \25 */
	{ 0,0,0,0,0,0,0 },			/* \26 */
	{ 0,0,0,0,0,0,0 },			/* \27 */
	{ 0,0,0,0,0,0,0 },			/* \30 */
	{ 0,0,0,0,0,0,0 },			/* \31 */
	{ 0,0,0,0,0,0,0 },			/* \32 */
	{ 0,0,0,0,0,0,0 },			/* \33 */
	{ 0,0,0,0,0,0,0 },			/* \34 */
	{ 0,0,0,0,0,0,0 },			/* \35 */
	{ 0,0,0,0,0,0,0 },			/* \36 */
	{ 0,0,0,0,0,0,0 },			/* \37 */
	{ 0,0,0,0,0,0,0 },			/* \40 */
	{ 034,034,034,010,0,034,034 },		/* ! */
	{ 0167,0167,042,0,0,0,0 },		/* " */
	{ 024,024,0177,024,0177,024,024 },	/* # */
	{ 076,0111,011,076,0110,0111,076 },	/* $ */
	{ 0107,045,027,010,0164,0122,0161 },	/* % */
	{ 014,022,014,016,0121,041,0116 },	/* & */
	{ 034,034,010,04,0,0,0 },		/* ' */
	{ 030,04,02,02,02,04,030 },		/* ( */
	{ 014,020,040,040,040,020,014 },	/* ) */
	{ 0,042,024,0177,024,042,0 },		/* * */
	{ 0,010,010,076,010,010,0 },		/* + */
	{ 0,0,0,034,034,010,04 },		/* , */
	{ 0,0,0,076,0,0,0 },			/* - */
	{ 0,0,0,0,034,034,034 },		/* . */
	{ 0100,040,020,010,04,02,01 },		/* / */
	{ 034,042,0101,0101,0101,042,034 },	/* 0 */
	{ 010,014,012,010,010,010,076 },	/* 1 */
	{ 076,0101,0100,076,01,01,0177 },	/* 2 */
	{ 076,0101,0100,076,0100,0101,076 },	/* 3 */
	{ 01,041,041,041,0177,040,040 },	/* 4 */
	{ 0177,01,01,077,0100,0101,076 },	/* 5 */
	{ 076,0101,01,077,0101,0101,076 },	/* 6 */
	{ 0177,041,020,010,04,04,04 },		/* 7 */
	{ 076,0101,0101,076,0101,0101,076 },	/* 8 */
	{ 076,0101,0101,0176,0100,0101,076 },	/* 9 */
	{ 010,034,010,0,010,034,010 },		/* : */
	{ 034,034,0,034,034,010,04 },		/* ; */
	{ 020,010,04,02,04,010,020 },		/* < */
	{ 0,0,076,0,076,0,0 },			/* = */
	{ 04,010,020,040,020,010,04 },		/* > */
	{ 076,0101,0100,070,010,0,010 },	/* ? */
	{ 076,0101,0135,0135,075,01,076 },	/* @ */
	{ 010,024,042,0101,0177,0101,0101 },	/* A */
	{ 077,0101,0101,077,0101,0101,077 },	/* B */
	{ 076,0101,01,01,01,0101,076 },		/* C */
	{ 077,0101,0101,0101,0101,0101,077 },	/* D */
	{ 0177,01,01,037,01,01,0177 },		/* E */
	{ 0177,01,01,037,01,01,01 },		/* F */
	{ 076,0101,01,0171,0101,0101,076 },	/* G */
	{ 0101,0101,0101,0177,0101,0101,0101 },	/* H */
	{ 034,010,010,010,010,010,034 },	/* I */
	{ 0100,0100,0100,0100,0101,0101,076 },	/* J */
	{ 041,021,011,07,011,021,041 },		/* K */
	{ 01,01,01,01,01,01,0177 },		/* L */
	{ 0101,0143,0125,0111,0101,0101,0101 },	/* M */
	{ 0101,0103,0105,0111,0121,0141,0101 },	/* N */
	{ 0177,0101,0101,0101,0101,0101,0177 },	/* O */
	{ 077,0101,0101,077,01,01,01 },		/* P */
	{ 076,0101,0101,0101,0121,041,0136 },	/* Q */
	{ 077,0101,0101,077,021,041,0101 },	/* R */
	{ 076,0101,01,076,0100,0101,076 },	/* S */
	{ 0177,010,010,010,010,010,010 },	/* T */
	{ 0101,0101,0101,0101,0101,0101,076 },	/* U */
	{ 0101,0101,0101,0101,042,024,010 },	/* V */
	{ 0101,0111,0111,0111,0111,0111,066 },	/* W */
	{ 0101,042,024,010,024,042,0101 },	/* X */
	{ 0101,042,024,010,010,010,010 },	/* Y */
	{ 0177,040,020,010,04,02,0177 },	/* Z */
	{ 076,02,02,02,02,02,076 },		/* [ */
	{ 01,02,04,010,020,040,0100 },		/* \ */
	{ 076,040,040,040,040,040,076 },	/* ] */
	{ 010,024,042,0,0,0,0 },		/* ^ */
	{ 0,0,0,0,0,0,0177 },			/* _ */
	{ 034,034,010,020,0,0,0 },		/* ` */
	{ 0,030,044,0102,0176,0102,0102 },	/* a */
	{ 0,076,0102,076,0102,0102,076 },	/* b */
	{ 0,074,0102,02,02,0102,074 },		/* c */
	{ 0,076,0102,0102,0102,0102,076 },	/* d */
	{ 0,0176,02,076,02,02,0176 },		/* e */
	{ 0,0176,02,076,02,02,02 },		/* f */
	{ 0,074,0102,02,0162,0102,074 },	/* g */
	{ 0,0102,0102,0176,0102,0102,0102 },	/* h */
	{ 0,020,020,020,020,020,020 },		/* i */
	{ 0,0100,0100,0100,0100,0102,074 },	/* j */
	{ 0,0102,042,036,022,042,0102 },	/* k */
	{ 0,02,02,02,02,02,0176 },		/* l */
	{ 0,0102,0146,0132,0102,0102,0102 },	/* m */
	{ 0,0102,0106,0112,0122,0142,0102 },	/* n */
	{ 0,074,0102,0102,0102,0102,074 },	/* o */
	{ 0,076,0102,0102,076,02,02 },		/* p */
	{ 0,074,0102,0102,0122,042,0134 },	/* q */
	{ 0,076,0102,0102,076,042,0102 },	/* r */
	{ 0,074,02,074,0100,0102,074 },		/* s */
	{ 0,0174,020,020,020,020,020 },		/* t */
	{ 0,0102,0102,0102,0102,0102,074 },	/* u */
	{ 0,0102,0102,0102,0102,044,030 },	/* v */
	{ 0,0102,0102,0102,0132,0146,0102 },	/* w */
	{ 0,0102,044,030,030,044,0102 },	/* x */
	{ 0,0104,050,020,020,020,020 },		/* y */
	{ 0,0176,040,020,010,04,0176 },		/* z */
	{ 034,02,02,03,02,02,034 },		/* { */
	{ 010,010,010,0,010,010,010 },		/* | */
	{ 034,040,040,0140,040,040,034 },	/* } */
	{ 06,0111,060,0,0,0,0 },		/* ~ */
	{ 0,0,0,0,0,0,0 },			/* \177 */
	{ 020,0,0,0,0104,0,050 },		/* \200 */
	{ 0,0176,0,0,0,03,040 },		/* \201 */
	{ 020,010,0,0,0,0,044 },		/* \202 */
	{ 0,0,0,0111,0,0,0 },			/* \203 */
	{ 037,014,020,010,0,0,0 },		/* \204 */
	{ 0,0104,0,0107,0177,0175,0 },		/* \205 */
	{ 0,0,0,0140,020,010,04 },		/* \206 */
	{ 0,0,0,044,0,0,0 },			/* \207 */
	{ 063,0,0,0,034,0114,020 },		/* \210 */
	{ 010,0,0,0,0,0104,0 },			/* \211 */
	{ 0107,0177,0,0175,0143,033,0123 },	/* \212 */
	{ 0103,073,0127,07,0,0175,0123 },	/* \213 */
	{ 073,023,0,0175,0123,073,067 },	/* \214 */
	{ 0113,047,0173,073,0,0175,0123 },	/* \215 */
	{ 027,0123,017,027,0,0103,027 },	/* \216 */
	{ 0123,017,0113,027,0,0175,0123 },	/* \217 */
	{ 023,0103,027,0103,0,0123,017 },	/* \220 */
	{ 0113,027,0,0175,033,0113,043 },	/* \221 */
	{ 0175,067,0123,047,0147,0113,0173 },	/* \222 */
	{ 073,0,0175,0175,063,07,0147 },	/* \223 */
	{ 027,0103,047,027,0,0175,0161 },	/* \224 */
	{ 031,0171,041,0101,031,0175,0171 },	/* \225 */
	{ 061,061,0145,0121,025,0175,025 },	/* \226 */
	{ 0101,041,031,0121,0175,0,07 },	/* \227 */
	{ 047,0113,073,027,063,0,0175 },	/* \230 */
	{ 021,0115,071,0101,0131,0111,0141 },	/* \231 */
	{ 0,0172,0127,0147,047,0172,033 },	/* \232 */
	{ 0113,043,0172,033,0113,043,0143 },	/* \233 */
	{ 072,0147,0173,072,0106,0,0 },		/* \234 */
	{ 0,010,0114,020,010,0160,0100 },	/* \235 */
	{ 0,0,030,0114,020,010,0160 },		/* \236 */
	{ 0120,0,0,04,0114,020,010 },		/* \237 */
	{ 0160,0110,0,0,024,0114,020 },		/* \240 */
	{ 010,0160,0160,0,0,014,0114 },		/* \241 */
	{ 020,010,0160,0150,0,0,041 },		/* \242 */
	{ 0,0,0,0177,0126,0,0114 },		/* \243 */
	{ 020,010,0177,0122,020,0114,020 },	/* \244 */
	{ 010,0,0,0,0,0177,0122 },		/* \245 */
	{ 010,0114,020,010,013,0,0 },		/* \246 */
	{ 0,0,0113,03,0177,0177,0177 },		/* \247 */
	{ 0177,0122,030,0114,020,010,013 },	/* \250 */
	{ 010,0,0,0,0113,05,0177 },		/* \251 */
	{ 0177,0177,0177,0122,04,0114,020 },	/* \252 */
	{ 010,013,04,0,0,0,0113 },		/* \253 */
	{ 01,0177,0177,0177,0177,0122,024 },	/* \254 */
	{ 0114,020,010,013,014,0,0 },		/* \255 */
	{ 0,0113,06,0177,0177,0177,0177 },	/* \256 */
	{ 0122,014,0114,020,010,013,02 },	/* \257 */
	{ 0,0,0,0113,02,0177,0177 },		/* \260 */
	{ 0177,053,0,053,0,0150,033 },		/* \261 */
	{ 045,016,01,040,020,010,0120 },	/* \262 */
	{ 01,027,0130,013,01,040,020 },		/* \263 */
	{ 010,013,062,0177,0177,0177,0140 },	/* \264 */
	{ 021,020,016,034,0114,020,010 },	/* \265 */
	{ 0120,01,027,0120,013,0124,0177 },	/* \266 */
	{ 0177,0177,013,012,0120,020,010 },	/* \267 */
	{ 013,0150,0177,0177,0177,0150,0121 },	/* \270 */
	{ 010,0130,025,0120,04,0110,0124 },	/* \271 */
	{ 01,014,020,010,045,0130,0125 },	/* \272 */
	{ 030,045,05,013,0171,0177,0177 },	/* \273 */
	{ 0177,013,077,0177,0177,0177,013 },	/* \274 */
	{ 0114,0,0,0,0140,021,030 },		/* \275 */
	{ 05,013,0,0177,0177,0177,053 },	/* \276 */
	{ 0,016,0100,0,0,0,054 },		/* \277 */
	{ 0,0,0,0,0160,0,027 },			/* \300 */
	{ 0141,0125,0150,033,0165,065,0145 },	/* \301 */
	{ 0150,0127,010,0150,0135,030,0140 },	/* \302 */
	{ 077,040,0137,0144,013,021,014 },	/* \303 */
	{ 020,010,013,0123,0177,0177,0177 },	/* \304 */
	{ 0115,053,0100,013,0131,0177,0177 },	/* \305 */
	{ 0177,0115,0176,0100,0,0,0 },		/* \306 */
	{ 0153,052,04,0177,0126,026,0124 },	/* \307 */
	{ 020,010,0177,026,0156,013,072 },	/* \310 */
	{ 0,0,0,0140,021,010,0177 },		/* \311 */
	{ 0126,026,0124,020,010,013,037 },	/* \312 */
	{ 0,0,0,0115,013,027,014 },		/* \313 */
	{ 020,010,013,0132,0177,0177,0177 },	/* \314 */
	{ 0115,0161,0156,077,037,0145,053 },	/* \315 */
	{ 0,013,04,0177,0177,0177,0115 },	/* \316 */
	{ 0155,075,0175,0111,0141,04,04 },	/* \317 */
	{ 0125,0150,033,0140,033,010,0165 },	/* \320 */
	{ 065,0145,0150,0131,030,0150,0127 },	/* \321 */
	{ 010,0110,0127,037,0150,0117,0105 },	/* \322 */
	{ 0170,076,0120,016,014,020,010 },	/* \323 */
	{ 05,013,070,0,0,0,0140 },		/* \324 */
	{ 021,010,0153,026,04,0150,061 },	/* \325 */
	{ 0152,0121,037,0136,050,0,0 },		/* \326 */
	{ 0,077,030,0150,061,0152,0121 },	/* \327 */
	{ 037,0155,075,0175,0111,0141,04 },	/* \330 */
	{ 04,0170,076,0141,0153,01,0160 },	/* \331 */
	{ 0130,04,016,0124,020,010,0165 },	/* \332 */
	{ 045,013,0124,0,0,0,0140 },		/* \333 */
	{ 021,010,0130,0161,010,0150,017 },	/* \334 */
	{ 050,074,061,020,0155,0127,061 },	/* \335 */
	{ 0155,075,0175,0111,0141,04,0125 },	/* \336 */
	{ 0150,033,0165,065,0145,0150,0135 },	/* \337 */
	{ 010,0146,067,0153,0132,04,04 },	/* \340 */
	{ 04,0146,0177,0153,02,0170,076 },	/* \341 */
	{ 020,0146,0150,0171,0145,03,0142 },	/* \342 */
	{ 0120,036,014,020,010,027,0170 },	/* \343 */
	{ 0153,061,0125,0140,0121,030,050 },	/* \344 */
	{ 0124,0116,014,020,010,010,024 },	/* \345 */
	{ 016,0161,0140,0177,010,037,0155 },	/* \346 */
	{ 061,0140,077,0160,037,0105,0146 },	/* \347 */
	{ 01,0155,075,0175,0111,0141,04 },	/* \350 */
	{ 04,0165,065,0145,0150,025,022 },	/* \351 */
	{ 024,050,035,022,04,0146,067 },	/* \352 */
	{ 0153,024,04,0146,0177,0153,0110 },	/* \353 */
	{ 0153,061,0125,0140,041,010,034 },	/* \354 */
	{ 016,0161,0140,0177,025,037,047 },	/* \355 */
	{ 061,0140,077,0160,037,013,0155 },	/* \356 */
	{ 075,0175,0141,0125,0150,033,0165 },	/* \357 */
	{ 065,0145,0150,0135,010,0146,067 },	/* \360 */
	{ 0153,0151,04,04,04,0153,061 },	/* \361 */
	{ 0125,061,021,0140,025,0,0176 },	/* \362 */
	{ 0145,0,0,0,0153,0174,04 },		/* \363 */
	{ 0150,0141,0153,065,0125,0140,041 },	/* \364 */
	{ 050,0124,016,014,020,010,016 },	/* \365 */
	{ 024,016,0127,04,0150,0141,0153 },	/* \366 */
	{ 065,0125,0140,041,061,020,016 },	/* \367 */
	{ 0,0171,0120,0177,0137,075,0150 },	/* \370 */
	{ 0141,0153,065,0125,0140,041,05 },	/* \371 */
	{ 013,017,014,020,010,013,013 },	/* \372 */
	{ 0137,0177,0177,0140,021,010,061 },	/* \373 */
	{ 0140,077,0160,037,0146,0155,075 },	/* \374 */
	{ 0175,0111,0141,04,04,041,0 },		/* \375 */
	{ 0,0,0,0,0,0,0 },			/* \376 */
	{ 0,0,0,030,0114,020,010 },		/* \377 */
};
const char blank=' ';
const char plot='#';

static void	banner(const char *, struct bann *);
static void	banfil(const char *, struct bann *);
static void	banset(char, struct bann *);
static void	banprt(struct bann *);

static void
banner(const char *s,struct bann *bufp)
{
	char c,*r;
	const char *p,*q;
	p=s;
	r=(char *)bufp;
	banset(blank,bufp);

	while((c= *s++)!=0){
	  if((s-p)>chpln)return;
	  q=ctbl[c&0377];
	  banfil(q,(struct bann *)r);
	  r+=pospch;
	}
}

static void
banfil(const char *c,struct bann *p)
{
	int i,j;
	for(i=0;i<nlines;i++){
	  for(j=0;j<pospch;j++){
	    if((c[i]&(1<<j))!=0)p->alpha[i][j] = plot;
	  }
	}
}

static void
banset(char c,struct bann *p)
{
	int i,j;
	for(i=0;i<nlines;i++)
	  for(j=0;j<pposs-1;j++)
	    p->alpha[i][j] = c;
}

static void
banprt(struct bann *ptr)
{
	int i,j;
	for(i=0;i<nlines;i++){
	  ptr->alpha[i][pposs-1]='\0';
	  for(j=pposs-2;j>=0;j--){
	    if(ptr->alpha[i][j]!=blank)break;
	    ptr->alpha[i][j]='\0';
	  }
	printf("%s\n",ptr->alpha[i]);
	}
	printf("\n");
}

int
banner_maker(char *string)
{
	banner(string,bp);
	banprt(bp);
	return 0;
}



