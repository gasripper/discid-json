/* --------------------------------------------------------------------------

   MusicBrainz -- The Internet music metadatabase

   Copyright (C) 2006 Matthias Friedrich
   Copyright (C) 2015 Ralf Sternberg

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

--------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <discid/discid.h>
#include <json-c/json.h>

#ifndef DISCID_HAVE_SPARSE_READ
#define discid_read_sparse(disc, dev, i) discid_read(disc, dev)
#endif

/*
 * Usage: discid [<device>]
 */
int main(int argc, char *argv[]) {
  DiscId *disc = discid_new();
  int i;
  char *device = NULL;
  json_object *root, *tracks;

  /* If we have an argument, use it as the device name */
  if (argc > 1) {
    device = argv[1];
  }
  /* read the disc in the specified (or default) disc drive */
  if (discid_read_sparse(disc, device, 0) == 0) {
    fprintf(stderr, "Error: %s\n", discid_get_error_msg(disc));
    return 1;
  }

  root = json_object_new_object();
  json_object_object_add(root, "first_track", json_object_new_int(discid_get_first_track_num(disc)));
  json_object_object_add(root, "last_track", json_object_new_int(discid_get_last_track_num(disc)));
  json_object_object_add(root, "sectors", json_object_new_int(discid_get_sectors(disc)));
  json_object_object_add(root, "freedb_id", json_object_new_string(discid_get_freedb_id(disc)));
  json_object_object_add(root, "musicbrainz_id", json_object_new_string(discid_get_id(disc)));
  json_object_object_add(root, "musicbrainz_submission_url", json_object_new_string(discid_get_submission_url(disc)));

  tracks = json_object_new_array();
  for (i = discid_get_first_track_num(disc); i <= discid_get_last_track_num(disc); i++) {
    json_object *track = json_object_new_object();
    json_object_object_add(track, "number", json_object_new_int(i));
    json_object_object_add(track, "offset", json_object_new_int(discid_get_track_offset(disc, i)));
    json_object_object_add(track, "length", json_object_new_int(discid_get_track_length(disc, i)));
    json_object_array_add(tracks, track);
  }
  json_object_object_add(root, "tracks", tracks);

  printf("%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

  discid_free(disc);
  json_object_put(root);
  return 0;
}
