#ifndef Macros_h
#define Macros_h


//##########################################################################################################//
#define ADD(object_name, level, name, ...) \
    static const debugType_t object_name = { level, name, MAKE_STRING_LIST(__VA_ARGS__) }; \
    MAKE_ENUM(object_name##_subtypes, __VA_ARGS__); 
    



#define _enum_format(x) x, 
// Hilfsmakro: Anzahl der Argumente zählen
#define COUNT_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__,10,9,8,7,6,5,4,3,2,1,0)

// Verkettung von Namen
#define CONCAT(a, b) CONCAT_(a, b)
#define CONCAT_(a, b) a##b

// Einzelne Enum-Makros für bis zu 10 Einträge
#define ENUM_1(e1) e1
#define ENUM_2(e1,e2) e1, e2
#define ENUM_3(e1,e2,e3) e1, e2, e3
#define ENUM_4(e1,e2,e3,e4) e1, e2, e3, e4
#define ENUM_5(e1,e2,e3,e4,e5) e1, e2, e3, e4, e5
#define ENUM_6(e1,e2,e3,e4,e5,e6) e1, e2, e3, e4, e5, e6
#define ENUM_7(e1,e2,e3,e4,e5,e6,e7) e1, e2, e3, e4, e5, e6, e7
#define ENUM_8(e1,e2,e3,e4,e5,e6,e7,e8) e1, e2, e3, e4, e5, e6, e7, e8
#define ENUM_9(e1,e2,e3,e4,e5,e6,e7,e8,e9) e1, e2, e3, e4, e5, e6, e7, e8, e9
#define ENUM_10(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10

// Haupt-Makro: Enum erzeugen
#define MAKE_ENUM(name, ...) \
    static const enum _##name { CONCAT(ENUM_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__) } name = (const _##name) 0u;


// Umwandlung eines Symbols in einen String (Stringifizierung)
#define STR(x) #x

// Für einzelne Anzahl an Argumenten
#define STRLIST_1(e1) { STR(e1) }
#define STRLIST_2(e1,e2) { STR(e1), STR(e2) }
#define STRLIST_3(e1,e2,e3) { STR(e1), STR(e2), STR(e3) }
#define STRLIST_4(e1,e2,e3,e4) { STR(e1), STR(e2), STR(e3), STR(e4) }
#define STRLIST_5(e1,e2,e3,e4,e5) { STR(e1), STR(e2), STR(e3), STR(e4), STR(e5) }
#define STRLIST_6(e1,e2,e3,e4,e5,e6) { STR(e1), STR(e2), STR(e3), STR(e4), STR(e5), STR(e6) }
#define STRLIST_7(e1,e2,e3,e4,e5,e6,e7) { STR(e1), STR(e2), STR(e3), STR(e4), STR(e5), STR(e6), STR(e7) }
#define STRLIST_8(e1,e2,e3,e4,e5,e6,e7,e8) { STR(e1), STR(e2), STR(e3), STR(e4), STR(e5), STR(e6), STR(e7), STR(e8) }
#define STRLIST_9(e1,e2,e3,e4,e5,e6,e7,e8,e9) { STR(e1), STR(e2), STR(e3), STR(e4), STR(e5), STR(e6), STR(e7), STR(e8), STR(e9) }
#define STRLIST_10(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10) { STR(e1), STR(e2), STR(e3), STR(e4), STR(e5), STR(e6), STR(e7), STR(e8), STR(e9), STR(e10) }

// Hauptmakro: erzeugt {"Red", "Green", ...}
#define MAKE_STRING_LIST(...) \
    CONCAT(STRLIST_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#endif


