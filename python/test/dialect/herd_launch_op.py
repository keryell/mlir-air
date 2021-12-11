
# (c) Copyright 2021 Xilinx Inc. All Rights Reserved.

# RUN: %PYTHON %s | FileCheck %s
# CHECK: %[[C0:.*]] = constant 2 : index
# CHECK: %[[C1:.*]] = constant 2 : index
# CHECK: air.launch_herd tile (%{{.*}}, %{{.*}}) in (%{{.*}}=%[[C0]], %{{.*}}=%[[C1]])attributes {sym_name = "pyHerd"} {
# CHECK:   air.herd_terminator
# CHECK: }
import air
from air.mlir.ir import *
from air.mlir.dialects import air as airdialect
#from air.mlir.dialects import arith
from air.mlir.dialects import builtin
from air.mlir.dialects import std

with Context() as ctx, Location.unknown():
  airdialect.register_dialect(ctx)

  module = Module.create()
  with InsertionPoint(module.body):
    ftype = FunctionType.get(
              [IntegerType.get_signless(32),
               IntegerType.get_signless(32)], [])
    fop = builtin.FuncOp("test", ftype)

    bb = fop.add_entry_block()
    with InsertionPoint(bb):
      idx_ty = IndexType.get()
      size_x = std.ConstantOp(idx_ty, IntegerAttr.get(idx_ty, 2))
      size_y = std.ConstantOp(idx_ty, IntegerAttr.get(idx_ty, 2))
      herd = airdialect.HerdLaunchOp("pyHerd", size_x.result, size_y.result, [])
      with InsertionPoint(herd.body):
        airdialect.HerdTerminatorOp()
      std.ReturnOp([])

print (module)