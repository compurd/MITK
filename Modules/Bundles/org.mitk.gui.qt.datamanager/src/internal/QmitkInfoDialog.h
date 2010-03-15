#ifndef QMITKINFODIALOG_H_
#define QMITKINFODIALOG_H_

#include <vector>

#include <QDialog>

//class QmitkDataStorageComboBox;
namespace mitk
{
  class DataNode;
}
class QTextBrowser;
class QLineEdit;

///
/// A small class which "eats" all Del-Key-pressed events on the node table.
/// When the Del Key is pressed selected nodes should be removed.
///
class QmitkInfoDialog : public QDialog
{
  Q_OBJECT

  public:
    QmitkInfoDialog( std::vector<mitk::DataNode*> _Nodes, QWidget * parent = 0, Qt::WindowFlags f = 0 );
  public slots:
    void OnSelectionChanged(const mitk::DataNode*);
    void OnSearchButtonClicked ( bool checked = false );
    void OnCancelButtonClicked ( bool checked = false );
    void KeyWordTextChanged(const QString & text);
  protected:
    bool eventFilter(QObject *obj, QEvent *event);
  protected:
    QLineEdit* m_KeyWord;
    QPushButton* m_SearchButton;
    QTextBrowser* m_TextBrowser;
};

#endif // QMITKINFODIALOG_H_