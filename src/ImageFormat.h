//  Copyright (C) 2001-2003 Matthew Landauer. All Rights Reserved.
//  
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of version 2 of the GNU General Public License as
//  published by the Free Software Foundation.
//
//  This program is distributed in the hope that it would be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  Further, any
//  license provided herein, whether implied or otherwise, is limited to
//  this program in accordance with the express provisions of the GNU
//  General Public License.  Patent licenses, if any, provided herein do not
//  apply to combinations of this program with other product or programs, or
//  any other product whatsoever.  This program is distributed without any
//  warranty that the program is delivered free of the rightful claim of any
//  third person by way of infringement or the like.  See the GNU General
//  Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write the Free Software Foundation, Inc., 59
//  Temple Place - Suite 330, Boston MA 02111-1307, USA.
//
// $Id: ImageFormat.h,v 1.7 2003/02/03 00:40:18 mlandauer Exp $

#ifndef _imageformat_h_
#define _imageformat_h_

#include <string>
#include "LibLoader.h"
#include "Path.h"

namespace Sp {

class Image;

//! Properties of an image format
/*!
  This is the abstract base class for the supported image formats.
*/
class ImageFormat
{
public:
	ImageFormat();
	virtual ~ImageFormat();
	//! Get the name of the image format
	virtual std::string getFormatString() = 0;
	//! Construct an instance of the related image object
	virtual Image* constructImage() = 0;
	//! Load all the image format plugins
	static void registerPlugins();
	//! Unload all the image format plugins
	static void deRegisterPlugins();
	//! Deduce the image format of a file from the magic number
	static ImageFormat* recogniseByMagic(const Path &path);
	//! Returns the image format from the name of the format
	static ImageFormat* recogniseByFormatString(const std::string &formatString);

private:
	//! Does the header come from a file of this format?
	/*!
		\param buf pointer to a buffer of length sizeToRecognise() which should contain the start of the file
		\return true if the header is recognised
	*/
	virtual bool recognise(unsigned char *buf) = 0;

	//! Return the number of bytes from the header that are required to recognise this image format
	virtual int getSizeToRecognise() = 0;
	static std::list<ImageFormat *> plugins;
	static void addPlugin(ImageFormat *plugin);
	static void removePlugin(ImageFormat *plugin);
	static ImageFormat* recentPlugin();
	static LibLoader loader;
};

}

#endif