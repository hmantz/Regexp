/*

Regular-expression matching library for Ardiuno.

Written by Nick Gammon.
Date: 30 April 2011

Heavily based on the Lua regular expression matching library written by Roberto Ierusalimschy. 
 
Adapted to run on the Arduino by Nick Gammon.

VERSION
 
 Version 1.0  - 30th April 2011
 
 
LICENSE


Copyright © 1994–2010 Lua.org, PUC-Rio.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation the rights to use, 
 copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
 and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 OR OTHER DEALINGS IN THE SOFTWARE. 

 
USAGE
 
 
 Find the first match of the regular expression "pattern" in the supplied string, starting at position "index".
 
 If found, returns REGEXP_MATCHED (1).
 
 Also match_start and match_len in the MatchState structure are set to the start offset and length of the match.
 
 The capture in the MatchState structure has the locations and lengths of each capture.
 
 If not found, returns REGEXP_NOMATCH (0).
 
 On a parsing error (eg. trailing % symbol) returns a negative number.
 
 
EXAMPLE OF CALLING
 
// -------------------------------------

#include <Regexp.h>
MatchState ms;

void setup ()
{
  
  Serial.begin (115200);
  
  ms.src = "Testing: answer=42";
  ms.src_len = strlen (ms.src);
  char result = regexp (ms, "(%a+)=(%d+)", 0);
  if (result == REGEXP_MATCHED)
  {
    // matching offsets in ms.capture
    
    Serial.print ("Captures: ");
    Serial.println (ms.level);
    
    for (int j = 0; j < ms.level; j++)
    {
      Serial.print ("Capture number: ");
      Serial.println (j + 1, DEC);
      Serial.print ("Text: '");
      const char * p = ms.capture [j].init;
      for (int k = 0; k < ms.capture [j].len; k++)
        Serial.print (*p++);
      
      Serial.println ("'");
      
    }
    
  }
  else if (result == REGEXP_NOMATCH)
  {
    // no match
  }
  else
  {
    // some sort of error
  }
}  // end of setup

void loop () {
}  // end of loop

 // -------------------------------------

 
PATTERNS
 
 Patterns
 
 The standard patterns (character classes) you can search for are:
 
 
 . --- (a dot) represents all characters. 
 %a --- all letters. 
 %c --- all control characters. 
 %d --- all digits. 
 %l --- all lowercase letters. 
 %p --- all punctuation characters. 
 %s --- all space characters. 
 %u --- all uppercase letters. 
 %w --- all alphanumeric characters. 
 %x --- all hexadecimal digits. 
 %z --- the character with hex representation 0x00 (null). 
 %% --- a single '%' character.
 
 %1 --- captured pattern 1.
 %2 --- captured pattern 2 (and so on).
 %f[s]  transition from not in set 's' to in set 's'.
 %b()   balanced pair ( ... ) 
 
 
 Important! - the uppercase versions of the above represent the complement of the class. 
 eg. %U represents everything except uppercase letters, %D represents everything except digits.
  
 There are some "magic characters" (such as %) that have special meanings. These are:
 
 
 ^ $ ( ) % . [ ] * + - ? 
 
 
 If you want to use those in a pattern (as themselves) you must precede them by a % symbol.
 
 eg. %% would match a single %
 
 You can build your own pattern classes (sets) by using square brackets, eg.
 
 
 [abc] ---> matches a, b or c
 [a-z] ---> matches lowercase letters (same as %l)
 [^abc] ---> matches anything except a, b or c
 [%a%d] ---> matches all letters and digits
 
 [%a%d_] ---> matches all letters, digits and underscore
 [%[%]] ---> matches square brackets (had to escape them with %)
 
 
 You can use pattern classes in the form %x in the set. 
 If you use other characters (like periods and brackets, etc.) they are simply themselves.
 
 You can specify a range of character inside a set by using simple characters (not pattern classes like %a) separated by a hyphen. 
 For example, [A-Z] or [0-9]. These can be combined with other things. For example [A-Z0-9] or [A-Z,.].
 
 A end-points of a range must be given in ascending order. That is, [A-Z] would match upper-case letters, but [Z-A] would not match anything.
 
 You can negate a set by starting it with a "^" symbol, thus [^0-9] is everything except the digits 0 to 9. 
 The negation applies to the whole set, so [^%a%d] would match anything except letters or digits. 
 In anywhere except the first position of a set, the "^" symbol is simply itself.
 
 Inside a set (that is a sequence delimited by square brackets) the only "magic" characters are:
 
 ] ---> to end the set, unless preceded by %
 % ---> to introduce a character class (like %a), or magic character (like "]")
 ^ ---> in the first position only, to negate the set (eg. [^A-Z)
 - ---> between two characters, to specify a range (eg. [A-F])
 
 
 Thus, inside a set, characters like "." and "?" are just themselves.
 
 The repetition characters, which can follow a character, class or set, are:
 
 
 +  ---> 1 or more repetitions (greedy)
 *  ---> 0 or more repetitions (greedy)
 
 -  ---> 0 or more repetitions (non greedy)
 ?  ---> 0 or 1 repetition only
 
 
 A "greedy" match will match on as many characters as possible, a non-greedy one will match on as few as possible.
 
 The standard "anchor" characters apply:
 
 
 ^  ---> anchor to start of subject string
 $  ---> anchor to end of subject string
 
 
 You can also use round brackets to specify "captures":
 
 
 You see (.*) here
 
 
 Here, whatever matches (.*) becomes the first pattern.
 
 You can also refer to matched substrings (captures) later on in an expression:
 
 eg. This would match:
 
 string = "You see dogs and dogs"
 regexp = "You see (.*) and %1"
 
  
 This example shows how you can look for a repetition of a word matched earlier, whatever that word was ("dogs" in this case).
 
 As a special case, an empty capture string returns as the captured pattern, the position of itself in the string. eg.
 
 string = "You see dogs and dogs"
 regexp = "You .* ()dogs .*"
 
 This would return a capture with an offset of 8, and a length of CAP_POSITION (-2)
 
 Finally you can look for nested "balanced" things (such as parentheses) by using %b, like this:
 
 
 string = "I see a (big fish (swimming) in the pond) here"
 regexp = "%b()"

 
 After %b you put 2 characters, which indicate the start and end of the balanced pair. 
 If it finds a nested version it keeps processing until we are back at the top level. 
 In this case the matching string was "(big fish (swimming) in the pond)".

 
*/


#include <setjmp.h>
#include <ctype.h>
#include <string.h>
#include "Regexp.h"

// for throwing errors
static jmp_buf regexp_error_return;
typedef unsigned char byte;

// error codes raised during regexp processing
static byte error (const char err)
{
  // does not return
  longjmp (regexp_error_return, err);
  return 0;  // keep compiler happy
}  // end of error

static int check_capture (MatchState *ms, int l) {
  l -= '1';
  if (l < 0 || l >= ms->level || ms->capture[l].len == CAP_UNFINISHED)
    return error(ERR_INVALID_CAPTURE_INDEX);
  return l;
} // end of check_capture

static int capture_to_close (MatchState *ms) {
  int level = ms->level;
  for (level--; level>=0; level--)
    if (ms->capture[level].len == CAP_UNFINISHED) return level;
  return error(ERR_INVALID_PATTERN_CAPTURE);
} // end of capture_to_close

static const char *classend (MatchState *ms, const char *p) {
  switch (*p++) {
    case REGEXP_ESC: {
      if (*p == '\0')
        error(ERR_MALFORMED_PATTERN_ENDS_WITH_ESCAPE);
      return p+1;
    }
    case '[': {
      if (*p == '^') p++;
      do {  /* look for a `]' */
        if (*p == '\0')
          error(ERR_MALFORMED_PATTERN_ENDS_WITH_RH_SQUARE_BRACKET);
        if (*(p++) == REGEXP_ESC && *p != '\0')
          p++;  /* skip escapes (e.g. `%]') */
      } while (*p != ']');
      return p+1;
    }
    default: {
      return p;
    }
  }
} // end of classend


static int match_class (int c, int cl) {
  int res;
  switch (tolower(cl)) {
    case 'a' : res = isalpha(c); break;
    case 'c' : res = iscntrl(c); break;
    case 'd' : res = isdigit(c); break;
    case 'l' : res = islower(c); break;
    case 'p' : res = ispunct(c); break;
    case 's' : res = isspace(c); break;
    case 'u' : res = isupper(c); break;
    case 'w' : res = isalnum(c); break;
    case 'x' : res = isxdigit(c); break;
    case 'z' : res = (c == 0); break;
    default: return (cl == c);
  }
  return (islower(cl) ? res : !res);
} // end of match_class


static int matchbracketclass (int c, const char *p, const char *ec) {
  int sig = 1;
  if (*(p+1) == '^') {
    sig = 0;
    p++;  /* skip the `^' */
  }
  while (++p < ec) {
    if (*p == REGEXP_ESC) {
      p++;
      if (match_class(c, uchar(*p)))
        return sig;
    }
    else if ((*(p+1) == '-') && (p+2 < ec)) {
      p+=2;
      if (uchar(*(p-2)) <= c && c <= uchar(*p))
        return sig;
    }
    else if (uchar(*p) == c) return sig;
  }
  return !sig;
} // end of matchbracketclass


static int singlematch (int c, const char *p, const char *ep) {
  switch (*p) {
    case '.': return 1;  /* matches any char */
    case REGEXP_ESC: return match_class(c, uchar(*(p+1)));
    case '[': return matchbracketclass(c, p, ep-1);
    default:  return (uchar(*p) == c);
  }
} // end of singlematch


static const char *match (MatchState *ms, const char *s, const char *p);


static const char *matchbalance (MatchState *ms, const char *s,
                                 const char *p) {
  if (*p == 0 || *(p+1) == 0)
    error(ERR_UNBALANCED_PATTERN);
  if (*s != *p) return NULL;
  else {
    int b = *p;
    int e = *(p+1);
    int cont = 1;
    while (++s < ms->src_end) {
      if (*s == e) {
        if (--cont == 0) return s+1;
      }
      else if (*s == b) cont++;
    }
  }
  return NULL;  /* string ends out of balance */
} //  end of matchbalance


static const char *max_expand (MatchState *ms, const char *s,
                               const char *p, const char *ep) {
  int i = 0;  /* counts maximum expand for item */
  while ((s+i)<ms->src_end && singlematch(uchar(*(s+i)), p, ep))
    i++;
  /* keeps trying to match with the maximum repetitions */
  while (i>=0) {
    const char *res = match(ms, (s+i), ep+1);
    if (res) return res;
    i--;  /* else didn't match; reduce 1 repetition to try again */
  }
  return NULL;
} // end of max_expand


static const char *min_expand (MatchState *ms, const char *s,
                               const char *p, const char *ep) {
  for (;;) {
    const char *res = match(ms, s, ep+1);
    if (res != NULL)
      return res;
    else if (s<ms->src_end && singlematch(uchar(*s), p, ep))
      s++;  /* try with one more repetition */
    else return NULL;
  }
} // end of min_expand


static const char *start_capture (MatchState *ms, const char *s,
                                  const char *p, int what) {
  const char *res;
  int level = ms->level;
  if (level >= MAXCAPTURES) error(ERR_TOO_MANY_CAPTURES);
  ms->capture[level].init = s;
  ms->capture[level].len = what;
  ms->level = level+1;
  if ((res=match(ms, s, p)) == NULL)  /* match failed? */
    ms->level--;  /* undo capture */
  return res;
} // end of start_capture


static const char *end_capture (MatchState *ms, const char *s,
                                const char *p) {
  int l = capture_to_close(ms);
  const char *res;
  ms->capture[l].len = s - ms->capture[l].init;  /* close capture */
  if ((res = match(ms, s, p)) == NULL)  /* match failed? */
    ms->capture[l].len = CAP_UNFINISHED;  /* undo capture */
  return res;
} // end of end_capture


static const char *match_capture (MatchState *ms, const char *s, int l) {
  size_t len;
  l = check_capture(ms, l);
  len = ms->capture[l].len;
  if ((size_t)(ms->src_end-s) >= len &&
      memcmp(ms->capture[l].init, s, len) == 0)
    return s+len;
  else return NULL;
} // end of match_capture


static const char *match (MatchState *ms, const char *s, const char *p) {
init: /* using goto's to optimize tail recursion */
  switch (*p) {
    case '(': {  /* start capture */
      if (*(p+1) == ')')  /* position capture? */
        return start_capture(ms, s, p+2, CAP_POSITION);
      else
        return start_capture(ms, s, p+1, CAP_UNFINISHED);
    }
    case ')': {  /* end capture */
      return end_capture(ms, s, p+1);
    }
    case REGEXP_ESC: {
      switch (*(p+1)) {
        case 'b': {  /* balanced string? */
          s = matchbalance(ms, s, p+2);
          if (s == NULL) return NULL;
          p+=4; goto init;  /* else return match(ms, s, p+4); */
        }
        case 'f': {  /* frontier? */
          const char *ep; char previous;
          p += 2;
          if (*p != '[')
            error(ERR_MISSING_LH_SQUARE_BRACKET_AFTER_ESC_F);
          ep = classend(ms, p);  /* points to what is next */
          previous = (s == ms->src) ? '\0' : *(s-1);
          if (matchbracketclass(uchar(previous), p, ep-1) ||
              !matchbracketclass(uchar(*s), p, ep-1)) return NULL;
          p=ep; goto init;  /* else return match(ms, s, ep); */
        }
        default: {
          if (isdigit(uchar(*(p+1)))) {  /* capture results (%0-%9)? */
            s = match_capture(ms, s, uchar(*(p+1)));
            if (s == NULL) return NULL;
            p+=2; goto init;  /* else return match(ms, s, p+2) */
          }
          goto dflt;  /* case default */
        }
      }
    }
    case '\0': {  /* end of pattern */
      return s;  /* match succeeded */
    }
    case '$': {
      if (*(p+1) == '\0')  /* is the `$' the last char in pattern? */
        return (s == ms->src_end) ? s : NULL;  /* check end of string */
      else goto dflt;
    }
    default: dflt: {  /* it is a pattern item */
      const char *ep = classend(ms, p);  /* points to what is next */
      int m = s<ms->src_end && singlematch(uchar(*s), p, ep);
      switch (*ep) {
        case '?': {  /* optional */
          const char *res;
          if (m && ((res=match(ms, s+1, ep+1)) != NULL))
            return res;
          p=ep+1; goto init;  /* else return match(ms, s, ep+1); */
        }
        case '*': {  /* 0 or more repetitions */
          return max_expand(ms, s, p, ep);
        }
        case '+': {  /* 1 or more repetitions */
          return (m ? max_expand(ms, s+1, p, ep) : NULL);
        }
        case '-': {  /* 0 or more repetitions (minimum) */
          return min_expand(ms, s, p, ep);
        }
        default: {
          if (!m) return NULL;
          s++; p=ep; goto init;  /* else return match(ms, s+1, ep); */
        }
      }
    }
  }
} // end of match



static const char *lmemfind (const char *s1, size_t l1,
                             const char *s2, size_t l2) {
  if (l2 == 0) return s1;  /* empty strings are everywhere */
  else if (l2 > l1) return NULL;  /* avoids a negative `l1' */
  else {
    const char *init;  /* to search for a `*s2' inside `s1' */
    l2--;  /* 1st char will be checked by `memchr' */
    l1 = l1-l2;  /* `s2' cannot be found after that */
    while (l1 > 0 && (init = (const char *)memchr(s1, *s2, l1)) != NULL) {
      init++;   /* 1st char is already checked */
      if (memcmp(init, s2+1, l2) == 0)
        return init-1;
      else {  /* correct `l1' and `s1' to try again */
        l1 -= init-s1;
        s1 = init;
      }
    }
    return NULL;  /* not found */
  }
}   // end of lmemfind

char regexp (MatchState & ms, const char * pattern, unsigned int index)
{
  // set up for throwing errors
  char rtn = setjmp (regexp_error_return);
  
  // error return
  if (rtn)
    return (rtn);
  
  if (index > ms.src_len) 
    index = ms.src_len;

  int anchor = (*pattern == '^') ? (pattern++, 1) : 0;
  const char *s1=ms.src + index;
  ms.src_end = ms.src+ms.src_len;
  do {
    const char *res;
    ms.level = 0;
    if ((res=match(&ms, s1, pattern)) != NULL) {
      ms.match_start = s1 - ms.src;
      ms.match_len = res - s1;
      return REGEXP_MATCHED;
    }
  } while (s1++ < ms.src_end && !anchor);
  
  return REGEXP_NOMATCH; // no match
  
} // end of regexp
