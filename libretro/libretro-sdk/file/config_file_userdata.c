/* Copyright  (C) 2010-2014 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this file (config_file_userdata.c).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <file/config_file_userdata.h>
#include <string/string_list.h>

#define get_array_setup() \
   struct config_file_userdata *usr = (struct config_file_userdata*)userdata; \
 \
   char key[2][256]; \
   snprintf(key[0], sizeof(key[0]), "%s_%s", usr->prefix[0], key_str); \
   snprintf(key[1], sizeof(key[1]), "%s_%s", usr->prefix[1], key_str); \
 \
   char *str = NULL; \
   bool got = config_get_string  (usr->conf, key[0], &str); \
   got = got || config_get_string(usr->conf, key[1], &str);

#define get_array_body(T) \
   if (got) \
   { \
      unsigned i; \
      struct string_list *list = string_split(str, " "); \
      *values = (T*)calloc(list->size, sizeof(T)); \
      for (i = 0; i < list->size; i++) \
         (*values)[i] = (T)strtod(list->elems[i].data, NULL); \
      *out_num_values = list->size; \
      string_list_free(list); \
      return true; \
   } \
   else \
   { \
      *values = (T*)calloc(num_default_values, sizeof(T)); \
      memcpy(*values, default_values, sizeof(T) * num_default_values); \
      *out_num_values = num_default_values; \
      return false; \
   }

int config_userdata_get_float(void *userdata, const char *key_str,
      float *value, float default_value)
{
   struct config_file_userdata *usr = (struct config_file_userdata*)userdata;

   char key[2][256];
   snprintf(key[0], sizeof(key[0]), "%s_%s", usr->prefix[0], key_str);
   snprintf(key[1], sizeof(key[1]), "%s_%s", usr->prefix[1], key_str);

   bool got = config_get_float  (usr->conf, key[0], value);
   got = got || config_get_float(usr->conf, key[1], value);

   if (!got)
      *value = default_value;
   return got;
}

int config_userdata_get_int(void *userdata, const char *key_str,
      int *value, int default_value)
{
   struct config_file_userdata *usr = (struct config_file_userdata*)userdata;

   char key[2][256];
   snprintf(key[0], sizeof(key[0]), "%s_%s", usr->prefix[0], key_str);
   snprintf(key[1], sizeof(key[1]), "%s_%s", usr->prefix[1], key_str);

   bool got = config_get_int  (usr->conf, key[0], value);
   got = got || config_get_int(usr->conf, key[1], value);

   if (!got)
      *value = default_value;
   return got;
}

int config_userdata_get_float_array(void *userdata, const char *key_str,
      float **values, unsigned *out_num_values,
      const float *default_values, unsigned num_default_values)
{
   get_array_setup()
   get_array_body(float)
}

int config_userdata_get_int_array(void *userdata, const char *key_str,
      int **values, unsigned *out_num_values,
      const int *default_values, unsigned num_default_values)
{
   get_array_setup()
   get_array_body(int)
}

int config_userdata_get_string(void *userdata, const char *key_str,
      char **output, const char *default_output)
{
   get_array_setup()

   if (got)
   {
      *output = str;
      return true; 
   }

   *output = strdup(default_output);
   return false;
}

void config_userdata_free(void *ptr)
{
   if (ptr)
      free(ptr);
}
