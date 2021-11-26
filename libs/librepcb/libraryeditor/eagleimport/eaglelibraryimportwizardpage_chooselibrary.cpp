/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * https://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include "eaglelibraryimportwizardpage_chooselibrary.h"

#include "eaglelibraryimportwizardcontext.h"
#include "ui_eaglelibraryimportwizardpage_chooselibrary.h"

#include <librepcb/common/dialogs/filedialog.h>
#include <librepcb/eagleimport/eaglelibraryimport.h>

#include <QtCore>
#include <QtWidgets>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

EagleLibraryImportWizardPage_ChooseLibrary::
    EagleLibraryImportWizardPage_ChooseLibrary(
        std::shared_ptr<EagleLibraryImportWizardContext> context,
        QWidget* parent) noexcept
  : QWizardPage(parent),
    mUi(new Ui::EagleLibraryImportWizardPage_ChooseLibrary),
    mContext(context) {
  mUi->setupUi(this);
  mUi->edtFilePath->setText("-");  // Workaround to force initial library load.
  connect(mUi->btnChooseFilePath, &QToolButton::clicked, this, [this]() {
    QString p = mUi->edtFilePath->text();
    if (p.trimmed().isEmpty()) {
      p = QDir::homePath();
    }
    p = FileDialog::getOpenFileName(this, tr("Choose file"), p, "*.lbr;;*");
    if (!p.isEmpty()) {
      mUi->edtFilePath->setText(p);
    }
  });
  connect(mUi->edtFilePath, &QLineEdit::textChanged, mContext.get(),
          &EagleLibraryImportWizardContext::setLbrFilePath,
          Qt::QueuedConnection);
  connect(mContext.get(), &EagleLibraryImportWizardContext::parseCompleted,
          mUi->lblMessages, &QLabel::setText);
  connect(mContext.get(), &EagleLibraryImportWizardContext::parseCompleted,
          this, &QWizardPage::completeChanged);
}

EagleLibraryImportWizardPage_ChooseLibrary::
    ~EagleLibraryImportWizardPage_ChooseLibrary() noexcept {
}

/*******************************************************************************
 *  General Methods
 ******************************************************************************/

void EagleLibraryImportWizardPage_ChooseLibrary::initializePage() {
  mUi->edtFilePath->setText(mContext->getLbrFilePath().toNative());
}

bool EagleLibraryImportWizardPage_ChooseLibrary::isComplete() const {
  return mContext->getImport().getTotalElementsCount() > 0;
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace library
}  // namespace librepcb
