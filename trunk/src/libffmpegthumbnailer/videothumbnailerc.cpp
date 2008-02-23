//    Copyright (C) 2007 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "videothumbnailerc.h"
#include "videothumbnailer.h"

#include <vector>
#include <stdexcept>

extern "C" video_thumbnailer* create_thumbnailer(void)
{
    video_thumbnailer* thumbnailer = new video_thumbnailer_struct();

    thumbnailer->thumbnailer            = new VideoThumbnailer();
    thumbnailer->thumbnail_size         = 128;
    thumbnailer->seek_percentage        = 10;
    thumbnailer->overlay_film_strip     = 0;
    thumbnailer->workaround_bugs        = 0;
    thumbnailer->thumbnail_image_type   = Png;
    
    return thumbnailer;
}

extern "C" void destroy_thumbnailer(video_thumbnailer* thumbnailer)
{
    VideoThumbnailer* videoThumbnailer = reinterpret_cast<VideoThumbnailer*>(thumbnailer->thumbnailer);
    delete videoThumbnailer;
    thumbnailer->thumbnailer = 0;
    
    delete thumbnailer;
}

extern "C" image_data* create_image_data(void)
{
    image_data* data        = new image_data();
    
    data->image_data_ptr    = 0;
    data->image_data_size   = 0;
    data->internal_data     = new std::vector<uint8_t>();
    
    return data;
}

extern "C" void destroy_image_data(image_data* data)
{
    data->image_data_ptr    = 0;
    data->image_data_size   = 0;
    
    std::vector<uint8_t>* dataVector = reinterpret_cast<std::vector<uint8_t>* >(data->internal_data);
    delete dataVector;
    data->internal_data     = 0;
    
    delete data;
}

extern "C" void generate_thumbnail_to_buffer(video_thumbnailer* thumbnailer, const char* movie_filename, image_data* generated_image_data)
{
    VideoThumbnailer* videoThumbnailer  = reinterpret_cast<VideoThumbnailer*>(thumbnailer->thumbnailer);
    std::vector<uint8_t>* dataVector    = reinterpret_cast<std::vector<uint8_t>* >(generated_image_data->internal_data);
    
    videoThumbnailer->setThumbnailSize(thumbnailer->thumbnail_size);
    videoThumbnailer->setSeekPercentage(thumbnailer->seek_percentage);
    videoThumbnailer->setFilmStripOverlay(thumbnailer->overlay_film_strip != 0);
    videoThumbnailer->setWorkAroundIssues(thumbnailer->workaround_bugs != 0);
    
    videoThumbnailer->generateThumbnail(movie_filename, thumbnailer->thumbnail_image_type, *dataVector);
    generated_image_data->image_data_ptr = &dataVector->front();
    generated_image_data->image_data_size = dataVector->size();
}

extern "C" void generate_thumbnail_to_file(video_thumbnailer* thumbnailer, const char* movie_filename, const char* output_fileName)
{
    VideoThumbnailer* videoThumbnailer = reinterpret_cast<VideoThumbnailer*>(thumbnailer->thumbnailer);
    
    videoThumbnailer->setThumbnailSize(thumbnailer->thumbnail_size);
    videoThumbnailer->setSeekPercentage(thumbnailer->seek_percentage);
    videoThumbnailer->setFilmStripOverlay(thumbnailer->overlay_film_strip != 0);
    videoThumbnailer->setWorkAroundIssues(thumbnailer->workaround_bugs != 0);
    
    videoThumbnailer->generateThumbnail(movie_filename, thumbnailer->thumbnail_image_type, output_fileName);
}