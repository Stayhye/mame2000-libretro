/******************************************************************************

  driver.c

  The list of all available drivers. Drivers have to be included here to be
  recognized by the executable.

  To save some typing, we use a hack here. This file is recursively #included
  twice, with different definitions of the DRIVER() macro. The first one
  declares external references to the drivers; the second one builds an array
  storing all the drivers.

******************************************************************************/

#include "driver.h"


#ifndef DRIVER_RECURSIVE

/* The "root" driver, defined so we can have &driver_##NAME in macros. */
struct GameDriver driver_0 =
{
	__FILE__,
	0,
	"",
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	NOT_A_DRIVER
};

#endif

#ifdef TINY_COMPILE
extern struct GameDriver TINY_NAME;

const struct GameDriver *drivers[] =
{
	&TINY_NAME,
	0	/* end of array */
};

#else

#ifndef DRIVER_RECURSIVE

#define DRIVER_RECURSIVE

/* step 1: declare all external references */
#define DRIVER(NAME) extern struct GameDriver driver_##NAME;
#define TESTDRIVER(NAME) extern struct GameDriver driver_##NAME;
#include "driver.c"

/* step 2: define the drivers[] array */
#undef DRIVER
#undef TESTDRIVER
#define DRIVER(NAME) &driver_##NAME,
#define TESTDRIVER(NAME)
const struct GameDriver *drivers[] =
{
#include "driver.c"
	0	/* end of array */
};

#else	/* DRIVER_RECURSIVE */

	/* Konami games */
	//DRIVER( pooyan )	/* GX320 (c) 1982 */
	//DRIVER( pooyans )	/* GX320 (c) 1982 Stern */
	//DRIVER( pootan )	/* bootleg */
	//DRIVER( timeplt )	/* GX393 (c) 1982 */
	//DRIVER( timepltc )	/* GX393 (c) 1982 + Centuri license*/
	//DRIVER( spaceplt )	/* bootleg */
	//DRIVER( psurge )	/* (c) 1988 unknown (NOT Konami) */
	//DRIVER( megazone )	/* GX319 (c) 1983 */
	//DRIVER( megaznik )	/* GX319 (c) 1983 + Interlogic / Kosuka */
	//DRIVER( pandoras )	/* GX328 (c) 1984 + Interlogic */
	//DRIVER( gyruss )	/* GX347 (c) 1983 */
	//DRIVER( gyrussce )	/* GX347 (c) 1983 + Centuri license */
	//DRIVER( venus )		/* bootleg */
	//DRIVER( trackfld )	/* GX361 (c) 1983 */
	//DRIVER( trackflc )	/* GX361 (c) 1983 + Centuri license */
	//DRIVER( hyprolym )	/* GX361 (c) 1983 */
	//DRIVER( hyprolyb )	/* bootleg */
	//DRIVER( rocnrope )	/* GX364 (c) 1983 */
	//DRIVER( rocnropk )	/* GX364 (c) 1983 + Kosuka */
	//DRIVER( circusc )	/* GX380 (c) 1984 */
	//DRIVER( circusc2 )	/* GX380 (c) 1984 */
	//DRIVER( circuscc )	/* GX380 (c) 1984 + Centuri license */
	//DRIVER( circusce )	/* GX380 (c) 1984 + Centuri license */
	//DRIVER( tp84 )		/* GX388 (c) 1984 */
	//DRIVER( tp84a )		/* GX388 (c) 1984 */
	//DRIVER( hyperspt )	/* GX330 (c) 1984 + Centuri */
	//DRIVER( hpolym84 )	/* GX330 (c) 1984 */
	//DRIVER( sbasketb )	/* GX405 (c) 1984 */
	//DRIVER( mikie )		/* GX469 (c) 1984 */
	//DRIVER( mikiej )	/* GX469 (c) 1984 */
	//DRIVER( mikiehs )	/* GX469 (c) 1984 */
	//DRIVER( roadf )		/* GX461 (c) 1984 */
	//DRIVER( roadf2 )	/* GX461 (c) 1984 */
	//DRIVER( yiear )		/* GX407 (c) 1985 */
	//DRIVER( yiear2 )	/* GX407 (c) 1985 */
	//DRIVER( kicker )	/* GX477 (c) 1985 */
	//DRIVER( shaolins )	/* GX477 (c) 1985 */
	//DRIVER( pingpong )	/* GX555 (c) 1985 */
	//DRIVER( gberet )	/* GX577 (c) 1985 */
	//DRIVER( rushatck )	/* GX577 (c) 1985 */
	//DRIVER( gberetb )	/* bootleg on different hardware */
	//DRIVER( mrgoemon )	/* GX621 (c) 1986 (Japan) */
	//DRIVER( jailbrek )	/* GX507 (c) 1986 */
	//DRIVER( finalizr )	/* GX523 (c) 1985 */
	//DRIVER( finalizb )	/* bootleg */
	//DRIVER( ironhors )	/* GX560 (c) 1986 */
	//DRIVER( dairesya )	/* GX560 (c) 1986 (Japan) */
	//DRIVER( farwest )
	//DRIVER( jackal )	/* GX631 (c) 1986 (World) */
	//DRIVER( topgunr )	/* GX631 (c) 1986 (US) */
	//DRIVER( jackalj )	/* GX631 (c) 1986 (Japan) */
	//DRIVER( topgunbl )	/* bootleg */
	//DRIVER( ddribble )	/* GX690 (c) 1986 */
	//DRIVER( contra )	/* GX633 (c) 1987 */
	//DRIVER( contrab )	/* bootleg */
	//DRIVER( contraj )	/* GX633 (c) 1987 (Japan) */
	//DRIVER( contrajb )	/* bootleg */
	//DRIVER( gryzor )	/* GX633 (c) 1987 */
	//DRIVER( combasc )	/* GX611 (c) 1988 */
	//DRIVER( combasct )	/* GX611 (c) 1987 */
	//DRIVER( combascj )	/* GX611 (c) 1987 (Japan) */
	//DRIVER( bootcamp )	/* GX611 (c) 1987 */
	//DRIVER( combascb )	/* bootleg */
	//DRIVER( rockrage )	/* GX620 (c) 1986 (World?) */
	//DRIVER( rockragj )	/* GX620 (c) 1986 (Japan) */
	//DRIVER( mx5000 )	/* GX669 (c) 1987 */
	//DRIVER( flkatck )	/* GX669 (c) 1987 (Japan) */
	//DRIVER( fastlane )	/* GX752 (c) 1987 */
	//DRIVER( labyrunr )	/* GX771 (c) 1987 (Japan) */
	//DRIVER( thehustl )	/* GX765 (c) 1987 (Japan) */
	//DRIVER( thehustj )	/* GX765 (c) 1987 (Japan) */
	//DRIVER( rackemup )	/* GX765 (c) 1987 */
	//DRIVER( battlnts )	/* GX777 (c) 1987 */
	//DRIVER( battlntj )	/* GX777 (c) 1987 (Japan) */
	//DRIVER( bladestl )	/* GX797 (c) 1987 */
	//DRIVER( bladstle )	/* GX797 (c) 1987 */
	//DRIVER( hcastle )	/* GX768 (c) 1988 */
	//DRIVER( hcastlea )	/* GX768 (c) 1988 */
	//DRIVER( hcastlej )	/* GX768 (c) 1988 (Japan) */
	//DRIVER( ajax )		/* GX770 (c) 1987 */
	//DRIVER( ajaxj )		/* GX770 (c) 1987 (Japan) */
	//DRIVER( scontra )	/* GX775 (c) 1988 */
	//DRIVER( scontraj )	/* GX775 (c) 1988 (Japan) */
	//DRIVER( thunderx )	/* GX873 (c) 1988 */
	//DRIVER( thnderxj )	/* GX873 (c) 1988 (Japan) */
	//DRIVER( mainevt )	/* GX799 (c) 1988 */
	//DRIVER( mainevt2 )	/* GX799 (c) 1988 */
	//DRIVER( ringohja )	/* GX799 (c) 1988 (Japan) */
	//DRIVER( devstors )	/* GX890 (c) 1988 */
	//DRIVER( devstor2 )	/* GX890 (c) 1988 */
	//DRIVER( devstor3 )	/* GX890 (c) 1988 */
	//DRIVER( garuka )	/* GX890 (c) 1988 (Japan) */
	//DRIVER( 88games )	/* GX861 (c) 1988 */
	//DRIVER( konami88 )	/* GX861 (c) 1988 */
	//DRIVER( hypsptsp )	/* GX861 (c) 1988 (Japan) */
	//DRIVER( gbusters )	/* GX878 (c) 1988 */
	//DRIVER( crazycop )	/* GX878 (c) 1988 (Japan) */
	//DRIVER( crimfght )	/* GX821 (c) 1989 (US) */
	//DRIVER( crimfgt2 )	/* GX821 (c) 1989 (World) */
	//DRIVER( crimfgtj )	/* GX821 (c) 1989 (Japan) */
	//DRIVER( spy )		/* GX857 (c) 1989 (US) */
	//DRIVER( bottom9 )	/* GX891 (c) 1989 */
	//DRIVER( bottom9n )	/* GX891 (c) 1989 */
	//DRIVER( blockhl )	/* GX973 (c) 1989 */
	//DRIVER( quarth )	/* GX973 (c) 1989 (Japan) */
	DRIVER( aliens )	/* GX875 (c) 1990 (World) */
	//DRIVER( aliens2 )	/* GX875 (c) 1990 (World) */
	//DRIVER( aliensu )	/* GX875 (c) 1990 (US) */
	//DRIVER( aliensj )	/* GX875 (c) 1990 (Japan) */
	//DRIVER( surpratk )	/* GX911 (c) 1990 (Japan) */
	//DRIVER( parodius )	/* GX955 (c) 1990 (Japan) */
	//DRIVER( rollerg )	/* GX999 (c) 1991 (US) */
	//DRIVER( rollergj )	/* GX999 (c) 1991 (Japan) */
//TESTDRIVER( xexex )		/* GX067 (c) 1991 */
	//DRIVER( simpsons )	/* GX072 (c) 1991 */
	DRIVER( simpsn2p )	/* GX072 (c) 1991 */
	//DRIVER( simps2pj )	/* GX072 (c) 1991 (Japan) */
	//DRIVER( vendetta )	/* GX081 (c) 1991 (Asia) */
	//DRIVER( vendett2 )	/* GX081 (c) 1991 (Asia) */
	//DRIVER( vendettj )	/* GX081 (c) 1991 (Japan) */
	//DRIVER( wecleman )	/* GX602 (c) 1986 */
	//DRIVER( hotchase )	/* GX763 (c) 1988 */
//TESTDRIVER( chqflag )	/* GX717 (c) 1988 */
//TESTDRIVER( chqflagj )	/* GX717 (c) 1988 (Japan) */
	//DRIVER( ultraman )	/* GX910 (c) 1991 Banpresto/Bandai */

	/* Konami "Nemesis hardware" games */
	//DRIVER( nemesis )	/* GX456 (c) 1985 */
	//DRIVER( nemesuk )	/* GX456 (c) 1985 */
	//DRIVER( konamigt )	/* GX561 (c) 1985 */
	//DRIVER( salamand )	/* GX587 (c) 1986 */
	//DRIVER( lifefrce )	/* GX587 (c) 1986 */
	//DRIVER( lifefrcj )	/* GX587 (c) 1986 */
	/* GX400 BIOS based games */
	//DRIVER( rf2 )		/* GX561 (c) 1985 */
	//DRIVER( twinbee )	/* GX412 (c) 1985 */
	//DRIVER( gradius )	/* GX456 (c) 1985 */
	//DRIVER( gwarrior )	/* GX578 (c) 1985 */

	/* Konami "Twin 16" games */
	//DRIVER( devilw )	/* GX687 (c) 1987 */
	//DRIVER( darkadv )	/* GX687 (c) 1987 */
	//DRIVER( majuu )		/* GX687 (c) 1987 (Japan) */
	//DRIVER( vulcan )	/* GX785 (c) 1988 */
	//DRIVER( gradius2 )	/* GX785 (c) 1988 (Japan) */
	//DRIVER( grdius2a )	/* GX785 (c) 1988 (Japan) */
	//DRIVER( grdius2b )	/* GX785 (c) 1988 (Japan) */
	//DRIVER( cuebrick )	/* GX903 (c) 1989 */
	//DRIVER( fround )	/* GX870 (c) 1988 */
	//DRIVER( hpuncher )	/* GX870 (c) 1988 (Japan) */
	//DRIVER( miaj )		/* GX808 (c) 1989 (Japan) */

	/* Konami Gradius III board */
	//DRIVER( gradius3 )	/* GX945 (c) 1989 (Japan) */
	//DRIVER( grdius3a )	/* GX945 (c) 1989 (Asia) */

	/* (some) Konami 68000 games */
	//DRIVER( mia )		/* GX808 (c) 1989 */
	//DRIVER( mia2 )		/* GX808 (c) 1989 */
	//DRIVER( tmnt )		/* GX963 (c) 1989 (US) */
	//DRIVER( tmht )		/* GX963 (c) 1989 (UK) */
	//DRIVER( tmntj )		/* GX963 (c) 1989 (Japan) */
	//DRIVER( tmht2p )	/* GX963 (c) 1989 (UK) */
	//DRIVER( tmnt2pj )	/* GX963 (c) 1990 (Japan) */
	//DRIVER( tmnt2po )	/* GX963 (c) 1989 (Oceania) */
	//DRIVER( punkshot )	/* GX907 (c) 1990 (US) */
	//DRIVER( punksht2 )	/* GX907 (c) 1990 (US) */
	//DRIVER( lgtnfght )	/* GX939 (c) 1990 (US) */
	//DRIVER( trigon )	/* GX939 (c) 1990 (Japan) */
	//DRIVER( blswhstl )	/* GX060 (c) 1991 */
	//DRIVER( detatwin )	/* GX060 (c) 1991 (Japan) */
//TESTDRIVER( glfgreat )	/* GX061 (c) 1991 */
//TESTDRIVER( glfgretj )	/* GX061 (c) 1991 (Japan) */
	//DRIVER( tmnt2 )		/* GX063 (c) 1991 (US) */
	//DRIVER( tmnt22p )	/* GX063 (c) 1991 (US) */
	//DRIVER( tmnt2a )	/* GX063 (c) 1991 (Asia) */
	//DRIVER( ssriders )	/* GX064 (c) 1991 (World) */
	//DRIVER( ssrdrebd )	/* GX064 (c) 1991 (World) */
	//DRIVER( ssrdrebc )	/* GX064 (c) 1991 (World) */
	//DRIVER( ssrdruda )	/* GX064 (c) 1991 (US) */
	//DRIVER( ssrdruac )	/* GX064 (c) 1991 (US) */
	DRIVER( ssrdrubc )	/* GX064 (c) 1991 (US) */
	//DRIVER( ssrdrabd )	/* GX064 (c) 1991 (Asia) */
	//DRIVER( ssrdrjbd )	/* GX064 (c) 1991 (Japan) */
	//DRIVER( xmen )		/* GX065 (c) 1992 (US) */
	//DRIVER( xmen6p )	/* GX065 (c) 1992 */
	DRIVER( xmen2pj )	/* GX065 (c) 1992 (Japan) */
	//DRIVER( thndrx2 )	/* GX073 (c) 1991 (Japan) */

/*
Konami System GX game list
1994.03 Racing Force (GX250)
1994.03 Golfing Greats 2 (GX218)
1994.04 Gokujou Parodius (GX321)
1994.07 Taisen Puzzle-dama (GX315)
1994.12 Soccer Super Stars (GX427)
1995.04 TwinBee Yahhoo! (GX424)
1995.08 Dragoon Might (GX417)
1995.12 Tokimeki Memorial Taisen Puzzle-dama (GX515)
1996.01 Salamander 2 (GX521)
1996.02 Sexy Parodius (GX533)
1996.03 Daisu-Kiss (GX535)
1996.03 Slam Dunk 2 / Run & Gun 2 (GX505)
1996.10 Taisen Tokkae-dama (GX615)
1996.12 Versus Net Soccer (GX627)
1997.07 Winning Spike (GX705)
1997.11 Rushing Heroes (GX?. Not released in Japan)
*/

//#endif /* NEOMAME */

#endif	/* DRIVER_RECURSIVE */

#endif	/* TINY_COMPILE */
