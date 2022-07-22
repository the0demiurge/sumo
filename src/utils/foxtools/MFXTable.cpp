/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2022 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    MFXTable.h
/// @author  Pablo Alvarez Lopez
/// @date    2022-07-21
///
// Button similar to FXButton but with the possibility of showing tooltips
/****************************************************************************/
#include <config.h>

#include "MFXTable.h"


FXDEFMAP(MFXTable) MFXTableMap[] = {
    FXMAPFUNC(SEL_ENTER,    0,  MFXTable::onEnter),
    FXMAPFUNC(SEL_LEAVE,    0,  MFXTable::onLeave),
};


// Object implementation
FXIMPLEMENT(MFXTable, FXHorizontalFrame, MFXTableMap, ARRAYNUMBER(MFXTableMap))


MFXTable::MFXTable(FXComposite *p , FXObject* tgt, FXSelector sel) :
    FXHorizontalFrame(p, GUIDesignAuxiliarFrame),
    myTarget(tgt),
    mySelector(sel) {
}


MFXTable::~MFXTable() {}


long
MFXTable::onEnter(FXObject* sender, FXSelector sel, void* ptr) {
    return FXHorizontalFrame::onEnter(sender, sel, ptr);
}


long
MFXTable::onLeave(FXObject* sender, FXSelector sel, void* ptr) {
    return FXHorizontalFrame::onLeave(sender, sel, ptr);
}


void
MFXTable::setItemText(FXint row, FXint column, const FXString& text, FXbool notify) {
    if ((row < myRows.size()) && (column < myColumns.size())) {
        myRows.at(row)->setText(column, text, notify);
    } else {
        throw ProcessError("Invalid row or column");
    }
}


FXString
MFXTable::getItemText(FXint row, FXint column) const {
    if ((row < myRows.size()) && (column < myColumns.size())) {
        return myRows.at(row)->getText(column);
    } else {
        throw ProcessError("Invalid row or column");
    }
}


FXint
MFXTable::getNumRows() const {
    return myRows.size();
}


FXint 
MFXTable::getCurrentRow() const {
    return currentRow;
}


FXbool
MFXTable::selectRow(FXint row, FXbool notify) {
    if (row < myRows.size()) {
        myRows.at(row)->select();
    } else {
        throw ProcessError("Invalid row");
    }
}


void
MFXTable::setCurrentItem(FXint row, FXint column, FXbool notify) {
    // CHECK
}


void 
MFXTable::setColumnText(FXint column, const FXString& text) {
    if (column < myColumns.size()) {
        myColumns.at(column)->setColumnLabel(text);
    } else {
        throw ProcessError("Invalid column");
    }
}


void
MFXTable::setTableSize(FXint numberRow, FXint numberColumn, FXbool notify) {
    // first clear table
    clearTable();
    // create columns
    for (int i = 0; i < numberColumn; i++) {
        myColumns.push_back(new Column(this));
    }
    // create rows
    for (int i = 0; i < numberRow; i++) {
        myRows.push_back(new Row(this));
    }
}


void 
MFXTable::setColumnWidth(FXint column, FXint columnWidth) {
    if (column < myColumns.size()) {
        myColumns.at(column)->getVerticalFrame()->setWidth(columnWidth);
    } else {
        throw ProcessError("Invalid column");
    }
}


FXTextField* 
MFXTable::getItem(FXint row, FXint col) const {
    if ((row < myRows.size()) && (col < myColumns.size())) {
        return myRows.at(row)->getTextField(col);
    }
    else {
        throw ProcessError("Invalid row or column");
    }
}


FXint 
MFXTable::getColumnWidth(FXint column) const {
    if (column < myColumns.size()) {
        return myColumns.at(column)->getVerticalFrame()->getWidth();
    }
    else {
        throw ProcessError("Invalid column");
    }

    // CHECK

}


void 
MFXTable::setDefColumnWidth(FXint columnWidth) {
    // CHECK
}


void
MFXTable::fitColumnsToContents(FXint column, FXint nc) {
    // CHECK
}


void 
MFXTable::setColumnHeaderMode(FXuint hint) {
    // CHECK
}


void
MFXTable::setRowHeaderMode(FXuint hint) {
    // CHECK
}


void
MFXTable::setRowHeaderWidth(FXint w) {
    // CHECK
}


int
MFXTable::getSelStartRow() {
    return -1;
    // CHECK
}

void 
MFXTable::clearTable() {
    // clear rows (always before columns)
    for (const auto& row : myRows) {
        delete row;
    }
    // clear columns
    for (const auto& column : myColumns) {
        delete column;
    }
    // drop rows and columns
    myRows.clear();
    myColumns.clear();
}


MFXTable::Column::Column(MFXTable* table) {
    // create vertical frame
    myVerticalFrame = new FXVerticalFrame(table, GUIDesignAuxiliarFrame);
    myVerticalFrame->create();
    // create label for column
    myLabel = new FXLabel(myVerticalFrame, "", nullptr, GUIDesignLabelAttribute);
    myLabel->create();
}


MFXTable::Column::~Column() {
    // destroy frame and label
    myVerticalFrame->destroy();
    myLabel->destroy();
    // delete vertical frame (this also delete Label and Row textFields)
    delete myVerticalFrame;
}


FXVerticalFrame*
MFXTable::Column::getVerticalFrame() const {
    return myVerticalFrame;
}


void
MFXTable::Column::setColumnLabel(const FXString& text) {
    myLabel->setText(text);
}


MFXTable::Column::Column() {}


MFXTable::Row::Row(MFXTable* table) {
    // build textFields
    for (int i = 0; i < table->myColumns.size(); i++) {
        auto textField = new FXTextField(table->myColumns.at(i)->getVerticalFrame(), GUIDesignTextFieldNCol, table->myTarget, table->mySelector, GUIDesignTextFielWidth50);
        textField->create();
        myTextFields.push_back(textField);
    }
}


MFXTable::Row::~Row() {
    // destroy all textFields
    for (const auto& textField : myTextFields) {
        textField->destroy();
    }
}


FXString 
MFXTable::Row::getText(int index) const {
    return myTextFields.at(index)->getText();
}


void 
MFXTable::Row::setText(int index, const FXString& text, FXbool notify) const {
    myTextFields.at(index)->setText(text, notify);
}


void MFXTable::Row::select() {
    // finish
}


MFXTable::Row::Row() {}

/****************************************************************************/