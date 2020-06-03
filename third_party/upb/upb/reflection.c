
#include "upb/reflection.h"

#include <string.h>
#include "upb/table.int.h"
#include "upb/msg.h"

#include "upb/port_def.inc"

static char field_size[] = {
  0,/* 0 */
  8, /* UPB_DESCRIPTOR_TYPE_DOUBLE */
  4, /* UPB_DESCRIPTOR_TYPE_FLOAT */
  8, /* UPB_DESCRIPTOR_TYPE_INT64 */
  8, /* UPB_DESCRIPTOR_TYPE_UINT64 */
  4, /* UPB_DESCRIPTOR_TYPE_INT32 */
  8, /* UPB_DESCRIPTOR_TYPE_FIXED64 */
  4, /* UPB_DESCRIPTOR_TYPE_FIXED32 */
  1, /* UPB_DESCRIPTOR_TYPE_BOOL */
  sizeof(upb_strview), /* UPB_DESCRIPTOR_TYPE_STRING */
  sizeof(void*), /* UPB_DESCRIPTOR_TYPE_GROUP */
  sizeof(void*), /* UPB_DESCRIPTOR_TYPE_MESSAGE */
  sizeof(upb_strview), /* UPB_DESCRIPTOR_TYPE_BYTES */
  4, /* UPB_DESCRIPTOR_TYPE_UINT32 */
  4, /* UPB_DESCRIPTOR_TYPE_ENUM */
  4, /* UPB_DESCRIPTOR_TYPE_SFIXED32 */
  8, /* UPB_DESCRIPTOR_TYPE_SFIXED64 */
  4, /* UPB_DESCRIPTOR_TYPE_SINT32 */
  8, /* UPB_DESCRIPTOR_TYPE_SINT64 */
};

/* Strings/bytes are special-cased in maps. */
static char _upb_fieldtype_to_mapsize[12] = {
  0,
  1,  /* UPB_TYPE_BOOL */
  4,  /* UPB_TYPE_FLOAT */
  4,  /* UPB_TYPE_INT32 */
  4,  /* UPB_TYPE_UINT32 */
  4,  /* UPB_TYPE_ENUM */
  sizeof(void*),  /* UPB_TYPE_MESSAGE */
  8,  /* UPB_TYPE_DOUBLE */
  8,  /* UPB_TYPE_INT64 */
  8,  /* UPB_TYPE_UINT64 */
  0,  /* UPB_TYPE_STRING */
  0,  /* UPB_TYPE_BYTES */
};

/** upb_msg *******************************************************************/

upb_msg *upb_msg_new(const upb_msgdef *m, upb_arena *a) {
  return _upb_msg_new(upb_msgdef_layout(m), a);
}

static bool in_oneof(const upb_msglayout_field *field) {
  return field->presence < 0;
}

static upb_msgval _upb_msg_getraw(const upb_msg *msg, const upb_fielddef *f) {
  const upb_msglayout_field *field = upb_fielddef_layout(f);
  const char *mem = UPB_PTR_AT(msg, field->offset, char);
  upb_msgval val = {0};
  int size = upb_fielddef_isseq(f) ? sizeof(void *)
                                   : field_size[field->descriptortype];
  memcpy(&val, mem, size);
  return val;
}

bool upb_msg_has(const upb_msg *msg, const upb_fielddef *f) {
  const upb_msglayout_field *field = upb_fielddef_layout(f);
  if (in_oneof(field)) {
    return _upb_getoneofcase_field(msg, field) == field->number;
  } else if (field->presence > 0) {
    return _upb_hasbit_field(msg, field);
  } else {
    UPB_ASSERT(field->descriptortype == UPB_DESCRIPTOR_TYPE_MESSAGE ||
               field->descriptortype == UPB_DESCRIPTOR_TYPE_GROUP);
    return _upb_msg_getraw(msg, f).msg_val != NULL;
  }
}

const upb_fielddef *upb_msg_whichoneof(const upb_msg *msg,
                                       const upb_oneofdef *o) {
  upb_oneof_iter i;
  const upb_fielddef *f;
  const upb_msglayout_field *field;
  const upb_msgdef *m = upb_oneofdef_containingtype(o);
  uint32_t oneof_case;

  /* This is far from optimal. */
  upb_oneof_begin(&i, o);
  if (upb_oneof_done(&i)) return false;
  f = upb_oneof_iter_field(&i);
  field = upb_fielddef_layout(f);
  oneof_case = _upb_getoneofcase_field(msg, field);

  return oneof_case ? upb_msgdef_itof(m, oneof_case) : NULL;
}

upb_msgval upb_msg_get(const upb_msg *msg, const upb_fielddef *f) {
  if (!upb_fielddef_haspresence(f) || upb_msg_has(msg, f)) {
    return _upb_msg_getraw(msg, f);
  } else {
    /* TODO(haberman): change upb_fielddef to not require this switch(). */
    upb_msgval val = {0};
    switch (upb_fielddef_type(f)) {
      case UPB_TYPE_INT32:
      case UPB_TYPE_ENUM:
        val.int32_val = upb_fielddef_defaultint32(f);
        break;
      case UPB_TYPE_INT64:
        val.int64_val = upb_fielddef_defaultint64(f);
        break;
      case UPB_TYPE_UINT32:
        val.uint32_val = upb_fielddef_defaultuint32(f);
        break;
      case UPB_TYPE_UINT64:
        val.uint64_val = upb_fielddef_defaultuint64(f);
        break;
      case UPB_TYPE_FLOAT:
        val.float_val = upb_fielddef_defaultfloat(f);
        break;
      case UPB_TYPE_DOUBLE:
        val.double_val = upb_fielddef_defaultdouble(f);
        break;
      case UPB_TYPE_BOOL:
        val.double_val = upb_fielddef_defaultbool(f);
        break;
      case UPB_TYPE_STRING:
      case UPB_TYPE_BYTES:
        val.str_val.data = upb_fielddef_defaultstr(f, &val.str_val.size);
        break;
      case UPB_TYPE_MESSAGE:
        val.msg_val = NULL;
        break;
    }
    return val;
  }
}

upb_mutmsgval upb_msg_mutable(upb_msg *msg, const upb_fielddef *f,
                              upb_arena *a) {
  const upb_msglayout_field *field = upb_fielddef_layout(f);
  upb_mutmsgval ret;
  char *mem = UPB_PTR_AT(msg, field->offset, char);
  bool wrong_oneof =
      in_oneof(field) && _upb_getoneofcase_field(msg, field) != field->number;

  memcpy(&ret, mem, sizeof(void*));

  if (a && (!ret.msg || wrong_oneof)) {
    if (upb_fielddef_ismap(f)) {
      const upb_msgdef *entry = upb_fielddef_msgsubdef(f);
      const upb_fielddef *key = upb_msgdef_itof(entry, UPB_MAPENTRY_KEY);
      const upb_fielddef *value = upb_msgdef_itof(entry, UPB_MAPENTRY_VALUE);
      ret.map = upb_map_new(a, upb_fielddef_type(key), upb_fielddef_type(value));
    } else if (upb_fielddef_isseq(f)) {
      ret.array = upb_array_new(a, upb_fielddef_type(f));
    } else {
      UPB_ASSERT(upb_fielddef_issubmsg(f));
      ret.msg = upb_msg_new(upb_fielddef_msgsubdef(f), a);
    }

    memcpy(mem, &ret, sizeof(void*));

    if (wrong_oneof) {
      *_upb_oneofcase_field(msg, field) = field->number;
    } else if (field->presence > 0) {
      _upb_sethas_field(msg, field);
    }
  }
  return ret;
}

void upb_msg_set(upb_msg *msg, const upb_fielddef *f, upb_msgval val,
                 upb_arena *a) {
  const upb_msglayout_field *field = upb_fielddef_layout(f);
  char *mem = UPB_PTR_AT(msg, field->offset, char);
  int size = upb_fielddef_isseq(f) ? sizeof(void *)
                                   : field_size[field->descriptortype];
  memcpy(mem, &val, size);
  if (field->presence > 0) {
    _upb_sethas_field(msg, field);
  } else if (in_oneof(field)) {
    *_upb_oneofcase_field(msg, field) = field->number;
  }
}

void upb_msg_clearfield(upb_msg *msg, const upb_fielddef *f) {
  const upb_msglayout_field *field = upb_fielddef_layout(f);
  char *mem = UPB_PTR_AT(msg, field->offset, char);
  int size = upb_fielddef_isseq(f) ? sizeof(void *)
                                   : field_size[field->descriptortype];

  if (field->presence > 0) {
    _upb_clearhas_field(msg, field);
  } else if (in_oneof(field)) {
    int32_t *oneof_case = _upb_oneofcase_field(msg, field);
    if (*oneof_case != field->number) return;
    *oneof_case = 0;
  }

  memset(mem, 0, size);
}

void upb_msg_clear(upb_msg *msg, const upb_msgdef *m) {
  _upb_msg_clear(msg, upb_msgdef_layout(m));
}

bool upb_msg_next(const upb_msg *msg, const upb_msgdef *m,
                  const upb_symtab *ext_pool, const upb_fielddef **out_f,
                  upb_msgval *out_val, size_t *iter) {
  size_t i = *iter;
  const upb_msgval zero = {0};
  const upb_fielddef *f;
  while ((f = _upb_msgdef_field(m, (int)++i)) != NULL) {
    upb_msgval val = _upb_msg_getraw(msg, f);

    /* Skip field if unset or empty. */
    if (upb_fielddef_haspresence(f)) {
      if (!upb_msg_has(msg, f)) continue;
    } else {
      upb_msgval test = val;
      if (upb_fielddef_isstring(f) && !upb_fielddef_isseq(f)) {
        /* Clear string pointer, only size matters (ptr could be non-NULL). */
        test.str_val.data = NULL;
      }
      /* Continue if NULL or 0. */
      if (memcmp(&test, &zero, sizeof(test)) == 0) continue;

      /* Continue on empty array or map. */
      if (upb_fielddef_ismap(f)) {
        if (upb_map_size(test.map_val) == 0) continue;
      } else if (upb_fielddef_isseq(f)) {
        if (upb_array_size(test.array_val) == 0) continue;
      }
    }

    *out_val = val;
    *out_f = f;
    *iter = i;
    return true;
  }
  *iter = i;
  return false;
}

bool _upb_msg_discardunknown(upb_msg *msg, const upb_msgdef *m, int depth) {
  size_t iter = UPB_MSG_BEGIN;
  const upb_fielddef *f;
  upb_msgval val;
  bool ret = true;

  if (--depth == 0) return false;

  _upb_msg_discardunknown_shallow(msg);

  while (upb_msg_next(msg, m, NULL /*ext_pool*/, &f, &val, &iter)) {
    const upb_msgdef *subm = upb_fielddef_msgsubdef(f);
    if (!subm) continue;
    if (upb_fielddef_ismap(f)) {
      const upb_fielddef *val_f = upb_msgdef_itof(subm, 2);
      const upb_msgdef *val_m = upb_fielddef_msgsubdef(val_f);
      upb_map *map = (upb_map*)val.map_val;
      size_t iter = UPB_MAP_BEGIN;

      if (!val_m) continue;

      while (upb_mapiter_next(map, &iter)) {
        upb_msgval map_val = upb_mapiter_value(map, iter);
        if (!_upb_msg_discardunknown((upb_msg*)map_val.msg_val, val_m, depth)) {
          ret = false;
        }
      }
    } else if (upb_fielddef_isseq(f)) {
      const upb_array *arr = val.array_val;
      size_t i, n = upb_array_size(arr);
      for (i = 0; i < n; i++) {
        upb_msgval elem = upb_array_get(arr, i);
        if (!_upb_msg_discardunknown((upb_msg*)elem.msg_val, subm, depth)) {
          ret = false;
        }
      }
    } else {
      if (!_upb_msg_discardunknown((upb_msg*)val.msg_val, subm, depth)) {
        ret = false;
      }
    }
  }

  return ret;
}

bool upb_msg_discardunknown(upb_msg *msg, const upb_msgdef *m, int maxdepth) {
  return _upb_msg_discardunknown(msg, m, maxdepth);
}

/** upb_array *****************************************************************/

upb_array *upb_array_new(upb_arena *a, upb_fieldtype_t type) {
  return _upb_array_new(a, type);
}

size_t upb_array_size(const upb_array *arr) {
  return arr->len;
}

upb_msgval upb_array_get(const upb_array *arr, size_t i) {
  upb_msgval ret;
  const char* data = _upb_array_constptr(arr);
  int lg2 = arr->data & 7;
  UPB_ASSERT(i < arr->len);
  memcpy(&ret, data + (i << lg2), 1 << lg2);
  return ret;
}

void upb_array_set(upb_array *arr, size_t i, upb_msgval val) {
  char* data = _upb_array_ptr(arr);
  int lg2 = arr->data & 7;
  UPB_ASSERT(i < arr->len);
  memcpy(data + (i << lg2), &val, 1 << lg2);
}

bool upb_array_append(upb_array *arr, upb_msgval val, upb_arena *arena) {
  if (!_upb_array_realloc(arr, arr->len + 1, arena)) {
    return false;
  }
  arr->len++;
  upb_array_set(arr, arr->len - 1, val);
  return true;
}

bool upb_array_resize(upb_array *arr, size_t size, upb_arena *arena) {
  return _upb_array_resize(arr, size, arena);
}

/** upb_map *******************************************************************/

upb_map *upb_map_new(upb_arena *a, upb_fieldtype_t key_type,
                     upb_fieldtype_t value_type) {
  return _upb_map_new(a, _upb_fieldtype_to_mapsize[key_type],
                      _upb_fieldtype_to_mapsize[value_type]);
}

size_t upb_map_size(const upb_map *map) {
  return _upb_map_size(map);
}

bool upb_map_get(const upb_map *map, upb_msgval key, upb_msgval *val) {
  return _upb_map_get(map, &key, map->key_size, val, map->val_size);
}

bool upb_map_set(upb_map *map, upb_msgval key, upb_msgval val,
                 upb_arena *arena) {
  return _upb_map_set(map, &key, map->key_size, &val, map->val_size, arena);
}

bool upb_map_delete(upb_map *map, upb_msgval key) {
  return _upb_map_delete(map, &key, map->key_size);
}

bool upb_mapiter_next(const upb_map *map, size_t *iter) {
  return _upb_map_next(map, iter);
}

bool upb_mapiter_done(const upb_map *map, size_t iter) {
  upb_strtable_iter i;
  UPB_ASSERT(iter != UPB_MAP_BEGIN);
  i.t = &map->table;
  i.index = iter;
  return upb_strtable_done(&i);
}

/* Returns the key and value for this entry of the map. */
upb_msgval upb_mapiter_key(const upb_map *map, size_t iter) {
  upb_strtable_iter i;
  upb_msgval ret;
  i.t = &map->table;
  i.index = iter;
  _upb_map_fromkey(upb_strtable_iter_key(&i), &ret, map->key_size);
  return ret;
}

upb_msgval upb_mapiter_value(const upb_map *map, size_t iter) {
  upb_strtable_iter i;
  upb_msgval ret;
  i.t = &map->table;
  i.index = iter;
  _upb_map_fromvalue(upb_strtable_iter_value(&i), &ret, map->val_size);
  return ret;
}

/* void upb_mapiter_setvalue(upb_map *map, size_t iter, upb_msgval value); */
