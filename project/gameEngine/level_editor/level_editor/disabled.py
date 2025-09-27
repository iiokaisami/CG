import bpy

#オペレータ 無効オプションを追加する
class MYADDON_OT_add_disable_flag(bpy.types.Operator):
    bl_idname = "myaddon.add_disable_flag"
    bl_label = "Export Disable フラグ追加"
    bl_description = "ゲームに出すか出さないかのフラグ（チェックボックス）を追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):
        obj = context.object
        print("オペレータ実行：disable_export追加")

        if obj is None:
            self.report({'WARNING'}, "オブジェクトが選択されていません")
            return {'CANCELLED'}

        # すでにプロパティがあれば何もしない
        if "disable_export" not in obj:
            obj["disable_export"] = True
        return {'FINISHED'}

#パネル 無効オプション
class OBJECT_PT_disable_flag(bpy.types.Panel):
    
    bl_idname = "OBJECT_PT_disable_flag"
    bl_label = "Export Flag"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self,context):
        layout = self.layout
        obj = context.object

        # カスタムプロパティがあるか確認して表示
        if "disable_export" in obj:
            layout.prop(obj, '["disable_export"]', text="Export to Game")
        else:
            layout.operator("myaddon.add_disable_flag", icon='ADD')