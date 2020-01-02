#pragma once

#ifndef GAURD_GSTC_H
#define GAURD_GSTC_H

namespace gstc 
{
	typedef unsigned char uchar;
	typedef unsigned short ushort;

	enum char_set
	{
		GSTC_FULL,
		GSTC_COMPRESSED
	};

	char convert(ushort intensity);
	char convert_compressed(ushort intensity);

	const ushort len = 70;
	const ushort len_compressed = 10;
	const double conv = len / 256.0;
	const double conv_compressed = len_compressed / 256.0;
	const uchar chars[len] = { ' ', '.', '\'', '`', '^', '"', ',', ':', ';', 'I',
		'l', '!', 'i', '>', '<', '~', '+', '_', '-', '?',
		']', '[', '}', '{', '1', ')', '(', '|', '\\', '/',
		't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z',
		'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z',
		'm', 'w', 'q', 'p', 'd', 'b', 'k', 'h', 'a', 'o',
		'*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$' };
	const uchar chars_compressed[len_compressed] = { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@' };

	char convert(ushort intensity) {
		return gstc::chars[(ushort)(intensity * gstc::conv)];
	}

	char convert_compressed(ushort intensity) {
		return gstc::chars_compressed[len_compressed - (ushort)(intensity * gstc::conv_compressed) - 1];
	}
}

#endif
