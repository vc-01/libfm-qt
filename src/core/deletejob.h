#ifndef FM2_DELETEJOB_H
#define FM2_DELETEJOB_H

#include "libfmqtglobals.h"
#include "fileoperationjob.h"
#include "filepath.h"
#include "gioptrs.h"

namespace Fm2 {

class LIBFM_QT_API DeleteJob : public Fm2::FileOperationJob {
    Q_OBJECT
public:
    DeleteJob(const FilePathList& paths): paths_{paths} {
    }

    DeleteJob(FilePathList&& paths): paths_{paths} {
    }

    ~DeleteJob() {
    }

    void run() override;
private:
    bool deleteFile(const FilePath& path, GFileInfoPtr inf, bool only_empty);
    bool deleteDirContent(const FilePath& path, GFileInfoPtr inf, bool only_empty);

private:
    FilePathList paths_;
};

} // namespace Fm2

#endif // FM2_DELETEJOB_H