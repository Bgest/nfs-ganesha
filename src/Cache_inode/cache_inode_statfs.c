/*
 * vim:expandtab:shiftwidth=8:tabstop=8:
 *
 * Copyright CEA/DAM/DIF  (2008)
 * contributeur : Philippe DENIEL   philippe.deniel@cea.fr
 *                Thomas LEIBOVICI  thomas.leibovici@cea.fr
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * ---------------------------------------
 */

/**
 * @defgroup Cache_inode Cache Inode
 * @{
 */

/**
 * @file cache_inode_statfs.c
 * @brief Get and eventually cache an entry.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef _SOLARIS
#include "solaris_port.h"
#endif                          /* _SOLARIS */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/file.h>           /* for having FNDELAY */
#include "HashTable.h"
#include "log.h"
#include "nfs23.h"
#include "nfs4.h"
#include "mount.h"
#include "nfs_core.h"
#include "cache_inode.h"
#include "nfs_exports.h"
#include "nfs_creds.h"
#include "nfs_proto_functions.h"
#include "nfs_tools.h"
#include "nfs_proto_tools.h"


cache_inode_status_t cache_inode_statfs(cache_entry_t * pentry,
                                        fsal_dynamicfsinfo_t * dynamicinfo,
                                        const struct req_op_context *req_ctx)
{
  fsal_status_t fsal_status;
  struct fsal_export *export;
  cache_inode_status_t status = CACHE_INODE_SUCCESS;

  /* Sanity check */
  if(!pentry || !dynamicinfo)
    {
      status = CACHE_INODE_INVALID_ARGUMENT;
      return status;
    }

  export = pentry->obj_handle->export;
  /* Get FSAL to get dynamic info */
  fsal_status = export->ops->get_fs_dynamic_info(export, req_ctx, dynamicinfo);
  if(FSAL_IS_ERROR(fsal_status))
    {
      status =  cache_inode_error_convert(fsal_status);
    }
  LogFullDebug(COMPONENT_CACHE_INODE,
               "cache_inode_statfs: dynamicinfo: {total_bytes = %"PRIu64", "
               "free_bytes = %"PRIu64", avail_bytes = %"PRIu64
               ", total_files = %"PRIu64", free_files = %"PRIu64
               ", avail_files = %"PRIu64"}",
               dynamicinfo->total_bytes, dynamicinfo->free_bytes,
               dynamicinfo->avail_bytes, dynamicinfo->total_files,
               dynamicinfo->free_files, dynamicinfo->avail_files);
  return status;
} /* cache_inode_statfs */
/** @} */