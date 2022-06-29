//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/SymbolTable.h"
#include "mlir/IR/TypeUtilities.h"

#include "air/Dialect/AIRRt/AIRRtOps.h"
#include "air/Dialect/AIRRt/AIRRtDialect.h"

using namespace mlir;
using namespace xilinx::airrt;

namespace xilinx {
namespace airrt {

//===----------------------------------------------------------------------===//
// ModuleMetadataOp
//===----------------------------------------------------------------------===//

void ModuleMetadataOp::print(OpAsmPrinter &p) {
  p.printOptionalAttrDictWithKeyword((*this)->getAttrs());
  p.printRegion(herds(), /*printEntryBlockArgs=*/false,
                /*printBlockTerminators=*/false);
}

ParseResult ModuleMetadataOp::parse(OpAsmParser &parser,
                                         OperationState &result) {
  if (parser.parseOptionalAttrDictWithKeyword(result.attributes))
    return failure();
  auto *body = result.addRegion();
  if (parser.parseRegion(*body, llvm::None, false))
    return failure();
  ModuleMetadataOp::ensureTerminator(*body, parser.getBuilder(),
                                     result.location);
  return success();
}

} // namespace airrt
} // namespace xilinx

#define GET_OP_CLASSES
#include "air/Dialect/AIRRt/AIRRtOps.cpp.inc"
