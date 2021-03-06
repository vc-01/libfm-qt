/*
 * Copyright (C) 2016 Hong Jen Yee (PCMan) <pcman.tw@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef __LIBFM_QT_FM2_FILE_INFO_H__
#define __LIBFM_QT_FM2_FILE_INFO_H__

#include <libfm/fm.h>
#include <QObject>
#include <QtGlobal>
#include "../libfmqtglobals.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <vector>
#include <utility>
#include <string>
#include <forward_list>

#include "gioptrs.h"
#include "filepath.h"
#include "iconinfo.h"
#include "mimetype.h"


namespace Fm {

class LIBFM_QT_API FileInfo {
public:

    explicit FileInfo();

    explicit FileInfo(const GFileInfoPtr& inf, const FilePath& parentDirPath);

    virtual ~FileInfo();

    bool canSetHidden() const {
        return isHiddenChangeable_;
    }

    bool canSetIcon() const {
        return isIconChangeable_;
    }

    bool canSetName() const {
        return isNameChangeable_;
    }

    bool canThumbnail() const;

    gid_t gid() const {
        return gid_;
    }

    uid_t uid() const {
        return uid_;
    }

    const char* filesystemId() const {
        return filesystemId_;
    }

    const std::shared_ptr<const IconInfo>& icon() const {
        return icon_;
    }

    const std::shared_ptr<const MimeType>& mimeType() const {
        return mimeType_;
    }

    time_t ctime() const {
        return ctime_;
    }


    time_t atime() const {
        return atime_;
    }

    time_t mtime() const {
        return mtime_;
    }

    const std::string& target() const {
        return target_;
    }

    bool isWritableDirectory() const {
        return (!isReadOnly_ && isDir());
    }

    bool isAccessible() const {
        return isAccessible_;
    }

    bool isExecutableType() const;

    bool isBackup() const {
        return isBackup_;
    }

    bool isHidden() const {
        // FIXME: we might treat backup files as hidden
        return isHidden_;
    }

    bool isUnknownType() const {
        return mimeType_->isUnknownType();
    }

    bool isDesktopEntry() const {
        return mimeType_->isDesktopEntry();
    }

    bool isText() const {
        return mimeType_->isText();
    }

    bool isImage() const {
        return mimeType_->isImage();
    }

    bool isMountable() const {
        return mimeType_->isMountable();
    }

    bool isShortcut() const {
        return isShortcut_;
    }

    bool isSymlink() const {
        return S_ISLNK(mode_) ? true : false;
    }

    bool isDir() const {
        return mimeType_->isDir();
    }

    bool isNative() const {
        return dirPath_ ? dirPath_.isNative() : path().isNative();
    }

    mode_t mode() const {
        return mode_;
    }

    uint64_t realSize() const {
        return blksize_ *blocks_;
    }

    uint64_t size() const {
        return size_;
    }

    const std::string& name() const {
        return name_;
    }

    const QString& displayName() const {
        return dispName_;
    }

    FilePath path() const {
        return dirPath_ ? dirPath_.child(name_.c_str()) : FilePath::fromPathStr(name_.c_str());
    }

    const FilePath& dirPath() const {
        return dirPath_;
    }

    void setFromGFileInfo(const GFileInfoPtr& inf, const FilePath& parentDirPath);

    const std::forward_list<std::shared_ptr<const IconInfo>>& emblems() const {
        return emblems_;
    }

private:
    std::string name_;
    QString dispName_;

    FilePath dirPath_;

    mode_t mode_;
    const char* filesystemId_;
    uid_t uid_;
    gid_t gid_;
    uint64_t size_;
    time_t mtime_;
    time_t atime_;
    time_t ctime_;

    uint64_t blksize_;
    uint64_t blocks_;

    std::shared_ptr<const MimeType> mimeType_;
    std::shared_ptr<const IconInfo> icon_;
    std::forward_list<std::shared_ptr<const IconInfo>> emblems_;

    std::string target_; /* target of shortcut or mountable. */

    bool isShortcut_ : 1; /* TRUE if file is shortcut type */
    bool isAccessible_ : 1; /* TRUE if can be read by user */
    bool isHidden_ : 1; /* TRUE if file is hidden */
    bool isBackup_ : 1; /* TRUE if file is backup */
    bool isNameChangeable_ : 1; /* TRUE if name can be changed */
    bool isIconChangeable_ : 1; /* TRUE if icon can be changed */
    bool isHiddenChangeable_ : 1; /* TRUE if hidden can be changed */
    bool isReadOnly_ : 1; /* TRUE if host FS is R/O */

    // std::vector<std::tuple<int, void*, void(void*)>> extraData_;
};


class LIBFM_QT_API FileInfoList: public std::vector<std::shared_ptr<const FileInfo>> {
public:

    bool isSameType() const;

    bool isSameFilesystem() const;

    FilePathList paths() const {
        FilePathList ret;
        for(auto& file: *this) {
            ret.push_back(file->path());
        }
        return ret;
    }
};


typedef std::pair<std::shared_ptr<const FileInfo>, std::shared_ptr<const FileInfo>> FileInfoPair;


}

Q_DECLARE_METATYPE(std::shared_ptr<const Fm::FileInfo>)


#endif // __LIBFM_QT_FM2_FILE_INFO_H__
