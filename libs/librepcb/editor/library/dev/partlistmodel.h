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

#ifndef LIBREPCB_EDITOR_PARTLISTMODEL_H
#define LIBREPCB_EDITOR_PARTLISTMODEL_H

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include <librepcb/core/library/dev/part.h>

#include <QtCore>

/*******************************************************************************
 *  Namespace / Forward Declarations
 ******************************************************************************/
namespace librepcb {
namespace editor {

class UndoCommand;
class UndoStack;

/*******************************************************************************
 *  Class PartListModel
 ******************************************************************************/

/**
 * @brief The PartListModel class
 */
class PartListModel final : public QAbstractTableModel {
  Q_OBJECT

public:
  enum Column {
    COLUMN_MPN,
    COLUMN_MANUFACTURER,
    COLUMN_ATTRIBUTES,
    COLUMN_ACTIONS,
    _COLUMN_COUNT
  };

  // Constructors / Destructor
  PartListModel() = delete;
  PartListModel(const PartListModel& other) noexcept = default;
  explicit PartListModel(QObject* parent = nullptr) noexcept;
  ~PartListModel() noexcept;

  // Setters
  void setInitialManufacturer(const SimpleString& value) noexcept;
  void setPartList(PartList* list) noexcept;
  void setUndoStack(UndoStack* stack) noexcept;

  // Slots
  void add(const QPersistentModelIndex& itemIndex) noexcept;
  void copy(const QPersistentModelIndex& itemIndex) noexcept;
  void remove(const QPersistentModelIndex& itemIndex) noexcept;
  void moveUp(const QPersistentModelIndex& itemIndex) noexcept;
  void moveDown(const QPersistentModelIndex& itemIndex) noexcept;

  // Inherited from QAbstractItemModel
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool setData(const QModelIndex& index, const QVariant& value,
               int role = Qt::EditRole) override;

  // Operator Overloadings
  PartListModel& operator=(const PartListModel& rhs) noexcept;

private:
  void partListEdited(const PartList& list, int index,
                      const std::shared_ptr<const Part>& part,
                      PartList::Event event) noexcept;
  void execCmd(UndoCommand* cmd);

private:  // Data
  PartList* mPartList;
  UndoStack* mUndoStack;
  QString mNewMpn;
  QString mNewManufacturer;

  // Slots
  PartList::OnEditedSlot mOnEditedSlot;
};

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace librepcb

#endif
