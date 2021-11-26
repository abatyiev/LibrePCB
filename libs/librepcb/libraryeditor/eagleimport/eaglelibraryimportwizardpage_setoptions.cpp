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
#include "eaglelibraryimportwizardpage_setoptions.h"

#include "../common/categorychooserdialog.h"
#include "../common/categorytreelabeltextbuilder.h"
#include "eaglelibraryimportwizardcontext.h"
#include "ui_eaglelibraryimportwizardpage_setoptions.h"

#include <librepcb/eagleimport/eaglelibraryimport.h>
#include <librepcb/workspace/library/workspacelibrarydb.h>
#include <librepcb/workspace/settings/workspacesettings.h>
#include <librepcb/workspace/workspace.h>

#include <QtCore>
#include <QtWidgets>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace library {
namespace editor {

using eagleimport::EagleLibraryImport;

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

EagleLibraryImportWizardPage_SetOptions::
    EagleLibraryImportWizardPage_SetOptions(
        std::shared_ptr<EagleLibraryImportWizardContext> context,
        QWidget* parent) noexcept
  : QWizardPage(parent),
    mUi(new Ui::EagleLibraryImportWizardPage_SetOptions),
    mContext(context) {
  mUi->setupUi(this);
  mUi->cbxAddNamePrefix->setText(mUi->cbxAddNamePrefix->text().arg(
      EagleLibraryImportWizardContext::NAME_PREFIX));
  setButtonText(QWizard::CommitButton, tr("&Import!"));
  setCommitPage(true);
  connect(mUi->cbxAddNamePrefix, &QCheckBox::toggled, mContext.get(),
          &EagleLibraryImportWizardContext::setAddNamePrefix);
  connect(
      mUi->btnChooseComponentCategory, &QToolButton::clicked, this, [this]() {
        ComponentCategoryChooserDialog dialog(mContext->getWorkspace(), this);
        if (dialog.exec() == QDialog::Accepted) {
          mContext->setComponentCategory(dialog.getSelectedCategoryUuid());
          updateComponentCategoryTreeLabel();
        }
      });
  connect(mUi->btnResetComponentCategory, &QToolButton::clicked, this,
          [this]() {
            mContext->setComponentCategory(tl::nullopt);
            updateComponentCategoryTreeLabel();
          });
  connect(mUi->btnChoosePackageCategory, &QToolButton::clicked, this, [this]() {
    PackageCategoryChooserDialog dialog(mContext->getWorkspace(), this);
    if (dialog.exec() == QDialog::Accepted) {
      mContext->setPackageCategory(dialog.getSelectedCategoryUuid());
      updatePackageCategoryTreeLabel();
    }
  });
  connect(mUi->btnResetPackageCategory, &QToolButton::clicked, this, [this]() {
    mContext->setPackageCategory(tl::nullopt);
    updatePackageCategoryTreeLabel();
  });
}

EagleLibraryImportWizardPage_SetOptions::
    ~EagleLibraryImportWizardPage_SetOptions() noexcept {
}

/*******************************************************************************
 *  General Methods
 ******************************************************************************/

void EagleLibraryImportWizardPage_SetOptions::initializePage() {
  mUi->cbxAddNamePrefix->setChecked(mContext->getAddNamePrefix());
  mUi->gbxComponentCategory->setVisible(
      (mContext->getImport().getCheckedSymbolsCount() +
       mContext->getImport().getCheckedComponentsCount() +
       mContext->getImport().getCheckedDevicesCount()) > 0);
  mUi->gbxPackageCategory->setVisible(
      mContext->getImport().getCheckedPackagesCount() > 0);
  updateComponentCategoryTreeLabel();
  updatePackageCategoryTreeLabel();
}

/*******************************************************************************
 *  Private Methods
 ******************************************************************************/

void EagleLibraryImportWizardPage_SetOptions::
    updateComponentCategoryTreeLabel() noexcept {
  ComponentCategoryTreeLabelTextBuilder builder(
      mContext->getWorkspace().getLibraryDb(),
      mContext->getWorkspace().getSettings().libraryLocaleOrder.get(),
      *mUi->lblComponentCategoryTree);
  builder.setHighlightLastLine(true);
  builder.setOneLine(true);
  builder.updateText(mContext->getComponentCategory());
}

void EagleLibraryImportWizardPage_SetOptions::
    updatePackageCategoryTreeLabel() noexcept {
  PackageCategoryTreeLabelTextBuilder builder(
      mContext->getWorkspace().getLibraryDb(),
      mContext->getWorkspace().getSettings().libraryLocaleOrder.get(),
      *mUi->lblPackageCategoryTree);
  builder.setHighlightLastLine(true);
  builder.setOneLine(true);
  builder.updateText(mContext->getPackageCategory());
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace library
}  // namespace librepcb
