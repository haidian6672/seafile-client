#ifndef SEAFILE_CLIENT_FILE_BROWSER_ADVANCEDSHAREDLINK_DIALOG_H
#define SEAFILE_CLIENT_FILE_BROWSER_ADVANCEDSHAREDLINK_DIALOG_H
#include <QDialog>
#include <QSpinBox>
#include <QGroupBox>

class CreateShareLinkRequest;
class QLineEdit;
struct SharedLinkInfo;
struct SharedLinkRequestParams;

class AdvancedSharedLinkDialog : public QDialog
{
    Q_OBJECT
public:
    AdvancedSharedLinkDialog(QWidget *parent,
                             const SharedLinkRequestParams &params);
    ~AdvancedSharedLinkDialog();

private slots:
    void onCopyText();
    void onOkBtnClicked();
    void onGenerateAdvancedSharedLinkSuccess(const SharedLinkInfo& shared_link_info);
    void onGenerateAdvancedSharedLinkFailed();

private:
    QString password_;
    QString password_again_;
    quint64 valid_days_;

    QGroupBox *pwd_group_box_;
    QGroupBox *expired_date_group_box_;
    QLineEdit *pwd_edit_;
    QLineEdit *pwd_edit_again_;
    QLineEdit *editor_;
    QSpinBox *expired_date_spin_box_;

    CreateShareLinkRequest *advanced_share_req_;
};

#endif