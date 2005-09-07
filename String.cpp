#include <string.h>
#include "String.h"
#include "FileUtils.h"
#include "MD5.h"

const char* g_szHTMLescapes[256] = {
	"&#0;", 0, 0, 0, 0, 0, 0, 0, 0, 0,               // 0-9
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 10-19
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 20-29
	0, 0, 0, 0, "&quot;", 0, 0, 0, "&amp;", "&#39;", // 30-39
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 40-49
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 50-59
	"&lt;", 0, "&gt;", 0, 0, 0, 0, 0, 0, 0,          // 60-69
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 70-79
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 80-89
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 90-99
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 100-109
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 110-119
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 120-129
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 130-139
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                    // 140-149
	0, 0, 0, "&trade;", 0, 0, 0, 0, 0, 0,            // 150-159
	"&nbsp;",   // 160
	"&iexcl;",  // 161
	"&cent;",   // 162
	"&pound;",  // 163
	"&curren;", // 164
	"&yen;",    // 165
	"&brvbar;", // 166
	"&sect;",   // 167
	"&uml;",    // 168
	"&copy;",   // 169
	"&ordf;",   // 170
	"&laquo;",  // 171
	"&not;",    // 172
	"&shy;",    // 173
	"&reg;",    // 174
	"&macr;",   // 175
	"&deg;",    // 176
	"&plusmn;", // 177
	"&sup2;",   // 178
	"&sup3;",   // 179
	"&acute;",  // 180
	"&micro;",  // 181
	"&para;",   // 182
	"&middot;", // 183
	"&cedil;",  // 184
	"&sup1;",   // 185
	"&ordm;",   // 186
	"&raquo;",  // 187
	"&frac14;", // 188
	"&frac12;", // 189
	"&frac34;", // 190
	"&iquest;", // 191
	"&Agrave;", // 192
	"&Aacute;", // 193
	"&Acirc;",  // 194
	"&Atilde;", // 195
	"&Auml;",   // 196
	"&Aring;",  // 197
	"&AElig;",  // 198
	"&Ccedil;", // 199
	"&Egrave;", // 200
	"&Eacute;", // 201
	"&Ecirc;",  // 202
	"&Euml;",   // 203
	"&Igrave;", // 204
	"&Iacute;", // 205
	"&Icirc;",  // 206
	"&Iuml;",   // 207
	"&ETH;",    // 208
	"&Ntilde;", // 209
	"&Ograve;", // 210
	"&Oacute;", // 211
	"&Ocirc;",  // 212
	"&Otilde;", // 213
	"&Ouml;",   // 214
	"&times;",  // 215
	"&Oslash;", // 216
	"&Ugrave;", // 217
	"&Uacute;", // 218
	"&Ucirc;",  // 219
	"&Uuml;",   // 220
	"&Yacute;", // 221
	"&THORN;",  // 222
	"&szlig;",  // 223
	"&agrave;", // 224
	"&aacute;", // 225
	"&acirc;",  // 226
	"&atilde;", // 227
	"&auml;",   // 228
	"&aring;",  // 229
	"&aelig;",  // 230
	"&ccedil;", // 231
	"&egrave;", // 232
	"&eacute;", // 233
	"&ecirc;",  // 234
	"&euml;",   // 235
	"&igrave;", // 236
	"&iacute;", // 237
	"&icirc;",  // 238
	"&iuml;",   // 239
	"&eth;",    // 240
	"&ntilde;", // 241
	"&ograve;", // 242
	"&oacute;", // 243
	"&ocirc;",  // 244
	"&otilde;", // 245
	"&ouml;",   // 246
	"&divide;", // 247
	"&oslash;", // 248
	"&ugrave;", // 249
	"&uacute;", // 250
	"&ucirc;",  // 251
	"&uuml;",   // 252
	"&yacute;", // 253
	"&thorn;",  // 254
	"&yuml;",   // 255
};

inline unsigned char* CString::strnchr(const unsigned char* src, unsigned char c, unsigned int iMaxBytes, unsigned char* pFill, unsigned int* piCount) const {
    for (unsigned int a = 0; a < iMaxBytes && *src; a++, src++) {
		if (pFill) {
			pFill[a] = *src;
		}

		if (*src == c) {
			if (pFill) {
				pFill[a +1] = 0;
			}

			if (piCount) {
				*piCount = a;
			}

			return (unsigned char*) src;
		}
	}

	if (pFill) {
		*pFill = 0;
	}

	if (piCount) {
		*piCount = 0;
	}

    return NULL;
}

int CString::CaseCmp(const CString& s) const {
	return strcasecmp(c_str(), s.c_str());
}

int CString::StrCmp(const CString& s) const {
	return strcmp(c_str(), s.c_str());
}

bool CString::WildCmp(const CString& sWild, const CString& sString) {
	// Written by Jack Handy - jakkhandy@hotmail.com
	const char *wild = sWild.c_str(), *CString = sString.c_str();
	const char *cp = NULL, *mp = NULL;

	while ((*CString) && (*wild != '*')) {
		if ((*wild != *CString) && (*wild != '?')) {
			return false;
		}

		wild++;
		CString++;
	}

	while (*CString) {
		if (*wild == '*') {
			if (!*++wild) {
				return true;
			}

			mp = wild;
			cp = CString+1;
		} else if ((*wild == *CString) || (*wild == '?')) {
			wild++;
			CString++;
		} else {
			wild = mp;
			CString = cp++;
		}
	}

	while (*wild == '*') {
		wild++;
	}

	return (*wild == 0);
}

bool CString::WildCmp(const CString& sWild) const {
	return CString::WildCmp(sWild, *this);
}

CString& CString::MakeUpper() {
	for (unsigned int a = 0; a < length(); a++) {
		char& c = (*this)[a];
		c = toupper(c);
	}

	return *this;
}

CString& CString::MakeLower() {
	for (unsigned int a = 0; a < length(); a++) {
		char& c = (*this)[a];
		c = tolower(c);
	}

	return *this;
}

CString CString::AsUpper() const {
	CString sRet = *this;

	for (unsigned int a = 0; a < length(); a++) {
		char& c = sRet[a];
		c = toupper(c);
	}

	return sRet;
}

CString CString::AsLower() const {
	CString sRet = *this;

	for (unsigned int a = 0; a < length(); a++) {
		char& c = sRet[a];
		c = tolower(c);
	}

	return sRet;
}

CString CString::Escape_n(EEscape eFrom, EEscape eTo) const {
	CString sRet;
	const char szHex[] = "0123456789ABCDEF";
	const unsigned char *pStart = (const unsigned char*) data();
	const unsigned char *p = (const unsigned char*) data();
	unsigned int iLength = length();
	sRet.reserve(iLength *3);
	unsigned char ch = 0;
	unsigned int iMaxLen = (eFrom == EHTML) ? 20 : 0;
	unsigned char pTmp[iMaxLen +1];
	unsigned int iCounted = 0;

	for (unsigned int a = 0; a < iLength; a++, p = pStart + a) {
		switch (eFrom) {
			case EHTML:
				if ((*p == '&') && (strnchr((unsigned char*) p, ';', iMaxLen, pTmp, &iCounted))) {
					if ((iCounted >= 3) && (pTmp[1] == '#')) {
						// do XML and HTML &#97; &#x3c
						int base = 10;

						if ((pTmp[2] & 0xDF) == 'X') {
							base = 16;
						}

						char* endptr = NULL;
						unsigned int b = strtol((const char*) (pTmp +2 + (base == 16)), &endptr, base);

						if ( ( *endptr == ';' ) && ( b <= 255 ) )
						{ // incase they do something like &#7777777777;
							ch = b;
							a += iCounted;
							break;
						}
					}

					for (unsigned int c = 0; c < 256; c++) {
						if (strcmp(g_szHTMLescapes[c], (const char*) &pTmp) == 0) {
							ch = c;
							break;
						}
					}

					if (ch > 0) {
						a += iCounted;
					} else {
						ch = *p;	 // Not a valid escape, just record the &
					}
				} else {
					ch = *p;
				}
				break;
			case EAscii:
				ch = *p;
				break;
			case EURL:
				if (*p == '%' && (a +2) < iLength && isxdigit(*(p +1)) && isxdigit(*(p +2))) {
					p++;
					if (isdigit(*p)) {
						ch = (*p - '0') << 4;
					} else {
						ch = (tolower(*p) - 'a' +10) << 4;
					}

					p++;
					if (isdigit(*p)) {
						ch |= (*p - '0');
					} else {
						ch |= (tolower(*p) - 'a' +10);
					}

					a += 2;
				} else if (pStart[a] == '+') {
					ch = ' ';
				} else {
					ch = *p;
				}

				break;
		}

		switch (eTo) {
			case EHTML:
				if (g_szHTMLescapes[ch]) {
					sRet += g_szHTMLescapes[ch];
				} else {
					sRet += ch;
				}

				break;
			case EAscii:
				sRet += ch;
				break;
			case EURL:
				if (isalnum(ch) || ch == '_' || ch == '.' || ch == '-') {
					sRet += ch;
				} else if (ch == ' ') {
					sRet += '+';
				} else {
					sRet += '%';
					sRet += szHex[ch >> 4];
					sRet += szHex[ch & 0xf];
				}

				break;
		}
	}

	sRet.reserve(0);
	return sRet;
}

CString CString::Escape_n(EEscape eTo) const {
	return Escape_n(EAscii, eTo);
}

CString& CString::Escape(EEscape eFrom, EEscape eTo) {
	return (*this = Escape_n(eFrom, eTo));
}

CString& CString::Escape(EEscape eTo) {
	return (*this = Escape_n(eTo));
}

unsigned int CString::Replace(const CString& sReplace, const CString& sWith) {
	return CString::Replace(*this, sReplace, sWith);
}

unsigned int CString::Replace(CString& sStr, const CString& sReplace, const CString& sWith) {
	unsigned int uRet = 0;
	size_type uPos = sStr.find(sReplace);

	while (uPos != npos) {
		sStr.replace(uPos, sReplace.length(), sWith);
		uPos = sStr.find(sReplace);
		uRet++;
	}

	return uRet;
}

CString CString::Token(unsigned int uPos, bool bRest, const CString& sSep) const {
	string sRet;
	const char* p = c_str();
	unsigned int uSepLen = sSep.length();

	if (uSepLen) {
		uSepLen--;
	}

	while (*p) {
		if (uPos) {
			if (strncmp(p, sSep.c_str(), sSep.length()) == 0) {
				uPos--;
				p += uSepLen;
			}
		} else {
			if (strncmp(p, sSep.c_str(), sSep.length()) == 0) {
				if (!bRest) {
					return sRet;
				}
			}

			sRet += *p;
		}

		p++;
	}

	return sRet;
}

CString CString::Ellipsize(unsigned int uLen) const {
	if (uLen >= size()) {
		return *this;
	}

	string sRet;

	// @todo this looks suspect
	if (uLen < 4) {
		for (unsigned int a = 0; a < uLen; a++) {
			sRet += ".";
		}

		return sRet;
	}

	sRet = substr(0, uLen -3) + "...";

	return sRet;
}

CString CString::Left(unsigned int uCount) const {
	uCount = (uCount > length()) ? length() : uCount;
	return substr(0, uCount);
}

CString CString::Right(unsigned int uCount) const {
	uCount = (uCount > length()) ? length() : uCount;
	return substr(length() - uCount, uCount);
}

VCString CString::Split(const CString& sDelim, bool bAllowEmpty) const {
	VCString vsRet;
	Split(sDelim, vsRet, bAllowEmpty);
	return vsRet;
}

unsigned int CString::Split(const CString& sDelim, VCString& vsRet, bool bAllowEmpty) const {
	vsRet.clear();
	CString sTmp = *this;

	while (sTmp.size()) {
		CString sTok = sTmp.Token(0, false, sDelim);
		CString sRest = sTmp.Token(1, true, sDelim);

		if (bAllowEmpty || !sTok.empty()) {
			vsRet.push_back(sTok);
		}

		if (bAllowEmpty && sRest.empty() && sTok.size() < sTmp.size()) {
			vsRet.push_back("");
		}

		sTmp = sRest;
	}

	return vsRet.size();
}

CString CString::Format(const CString& sFormatStr, ...) {
	return "";
}

unsigned long CString::Base64Decode(CString& sRet) const {
	const char* in = c_str();
	char c, c1, *p;
	unsigned long i;
	unsigned long uLen = size();
	char* out = (char*) malloc(size() +1);
 
	for (i = 0, p = out; i < uLen; i++) {
		c = (char)base64_table[(unsigned char)in[i++]];
		c1 = (char)base64_table[(unsigned char)in[i++]];
		*p++ = (c << 2) | ((c1 >> 4) & 0x3);
 
		if (i < uLen) {
			if (in[i] == '=') {
				break;
			}
			c = (char)base64_table[(unsigned char)in[i]];
			*p++ = ((c1 << 4) & 0xf0) | ((c >> 2) & 0xf);
		}
 
		if (++i < uLen) {
			if (in[i] == '=') {
				break;
			}
			*p++ = ((c << 6) & 0xc0) | (char)base64_table[(unsigned char)in[i]];
		}
	}
 
	*p = '\0';
	unsigned long uRet = p - out;
	sRet = out;
	free(out);

	return uRet;
}

CString CString::MD5() const {
	return (const char*) CMD5(*this);
}

CString CString::ToString(char c) { stringstream s; s << c; return s.str(); }
CString CString::ToString(unsigned char c) { stringstream s; s << c; return s.str(); }
CString CString::ToString(short i) { stringstream s; s << i; return s.str(); }
CString CString::ToString(unsigned short i) { stringstream s; s << i; return s.str(); }
CString CString::ToString(int i) { stringstream s; s << i; return s.str(); }
CString CString::ToString(unsigned int i) { stringstream s; s << i; return s.str(); }
CString CString::ToString(long i) { stringstream s; s << i; return s.str(); }
CString CString::ToString(unsigned long i) { stringstream s; s << i; return s.str(); }
CString CString::ToString(long long i) { stringstream s; s << i; return s.str(); }
CString CString::ToString(unsigned long long i) { stringstream s; s << i; return s.str(); }
CString CString::ToString(double i) { stringstream s; s << i; return s.str(); }
CString CString::ToString(float i) { stringstream s; s << i; return s.str(); }

CString CString::ToPercent(double d) {
	char szRet[32];
	snprintf(szRet, 32, "%.02f%%", d);
	return szRet;
}

CString CString::ToKBytes(double d) {
	char szRet[32];
	snprintf(szRet, 32, "%.0f K/s", d);
	return szRet;
}

bool CString::ToBool() const { return (!Trim_n().Trim_n("0").empty() && CaseCmp("false") != 0); }
short CString::ToShort() const { return strtoul(this->c_str(), (char**) NULL, 10); }
unsigned short CString::ToUShort() const { return strtoul(this->c_str(), (char**) NULL, 10); }
unsigned int CString::ToUInt() const { return strtoul(this->c_str(), (char**) NULL, 10); }
int CString::ToInt() const { return strtoul(this->c_str(), (char**) NULL, 10); }
unsigned long CString::ToULong() const { return strtoul(c_str(), NULL, 10); }
unsigned long long CString::ToULongLong() const { return strtoull(c_str(), NULL, 10); }
long long CString::ToLongLong() const { return strtoll(c_str(), NULL, 10); }
double CString::ToDouble() const { return strtod(c_str(), NULL); }


bool CString::Trim(const CString& s) {
	bool bLeft = TrimLeft(s);
	return (TrimRight(s) || bLeft);
}

bool CString::TrimLeft(const CString& s) {
	bool bRet = false;

	while (length() && s.find(Left(1)) != CString::npos) {
		LeftChomp();
		bRet = true;
	}

	return bRet;
}

bool CString::TrimRight(const CString& s) {
	bool bRet = false;

	while (length() && s.find(Right(1)) != CString::npos) {
		RightChomp();
		bRet = true;
	}

	return bRet;
}

CString CString::Trim_n(const CString& s) const {
	CString sRet = *this;
	sRet.Trim(s);
	return sRet;
}

CString CString::TrimLeft_n(const CString& s) const {
	CString sRet = *this;
	sRet.TrimLeft(s);
	return sRet;
}

CString CString::TrimRight_n(const CString& s) const {
	CString sRet = *this;
	sRet.TrimRight(s);
	return sRet;
}

bool CString::LeftChomp(unsigned int uLen) {
	bool bRet = false;

	while ((uLen--) && (length())) {
		erase(0, 1);
		bRet = true;
	}

	return bRet;
}

bool CString::RightChomp(unsigned int uLen) {
	bool bRet = false;

	while ((uLen--) && (length())) {
		erase(length() -1);
		bRet = true;
	}

	return bRet;
}

//////////////// MCString ////////////////
int MCString::WriteToDisk(const CString& sPath, mode_t iMode) {
	CFile cFile(sPath);
	if (!cFile.Open(O_WRONLY|O_CREAT|O_TRUNC, iMode)) {
		return MCS_EOPEN;
	}

	for (MCString::iterator it = this->begin(); it != this->end(); it++) {
		CString sKey = it->first;
		CString sValue = it->second;
		if (!WriteFilter(sKey, sValue)) {
			return MCS_EWRITEFIL;
		}

		if (sKey.empty()) {
			continue;
		}

		if (cFile.Write(Encode(sKey) + " " +  Encode(sValue) + "\n") <= 0) {
			return MCS_EWRITE;
		}
	}

	cFile.Close();

	return MCS_SUCCESS;
}

int MCString::ReadFromDisk(const CString& sPath, mode_t iMode) {
	clear();
	CFile cFile(sPath);
	if (!cFile.Open(O_RDONLY|O_CREAT, iMode)) {
		return MCS_EOPEN;
	}

	CString sBuffer;

	while (cFile.ReadLine(sBuffer)) {
		sBuffer.Trim();
		CString sKey = sBuffer.Token(0);
		CString sValue = sBuffer.Token(1);
		Decode(sKey);
		Decode(sValue);

		if (!ReadFilter(sKey, sValue))
			return MCS_EREADFIL;

		(*this)[sKey] = sValue;
	}
	cFile.Close();

	return MCS_SUCCESS;
}


static const char hexdigits[] = "0123456789abcdef";

CString& MCString::Encode(CString& sValue) {
	CString sTmp;
	for (CString::iterator it = sValue.begin(); it != sValue.end(); it++) {
		if (isalnum(*it)) {
			sTmp += *it;
		} else {
			sTmp += "%";
			sTmp += hexdigits[*it >> 4];
			sTmp += hexdigits[*it & 0xf];
			sTmp += ";";
		}
	}
	sValue = sTmp;
	return sValue;
}

CString& MCString::Decode(CString& sValue) {
	const char *pTmp = sValue.c_str();
	char *endptr;
	CString sTmp;

	while(*pTmp) {
		if (*pTmp != '%') {
			sTmp += *pTmp++;
		} else {
			char ch = (char )strtol(((const char *)pTmp + 1), &endptr, 16);
			if (*endptr == ';') {
				sTmp += ch;
				pTmp = ++endptr;
			} else {
				sTmp += *pTmp++;
			}
		}
	}

	sValue = sTmp;
	return sValue;
}














