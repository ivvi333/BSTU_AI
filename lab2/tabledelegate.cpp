#include "tabledelegate.h"

#include <QLineEdit>
#include <QIntValidator>

QWidget *TableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    QIntValidator *validator = new QIntValidator(0, 1, lineEdit);
    lineEdit->setValidator(validator);
    return lineEdit;
}
