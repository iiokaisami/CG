import bpy
import math
import bpy_extras
import gpu
import gpu_extras.batch
import copy
import mathutils
import json

# モジュールのインポート
from .stretch_vertex import MYADDON_OT_stretch_vertex
from .add_filename import MYADDON_OT_add_filename, OBJECT_PT_file_name
from .create_ico_sphere import MYADDON_OT_create_ico_spher
from .export_scene import MYADDON_OT_export_scene
from .collider import MYADDON_OT_add_collider, OBJECT_PT_collider
from .draw_collider import DrawCollider
from .my_menu import TOPBAR_MT_my_menu  

bl_info = {
    "name":"level_editor",
    "author":"Isami Iioka",
    "version":(1,0),
    "blender":(3,3,1),
    "location":"",
    "description":"Level_Editor",
    "warning":"",
    "wiki_url":"",
    "tracker_url":"",
    "category":"Object"
}

# メニュー項目追加
def draw_menu_manual(self,context):
    #self : 世帯出し元のクラスインスタンス。C++でいうthisポインタ
    #context : カーソルを合わせた時のポップアップのカスタマイズなどに使用

    #トップバーの「エディターメニュー」に項目(オペレータ)を追加
    self.layout.operator("wm.url_open_preset", text="Manual", icon='HELP')

#Add-On有効化時コールバック
def register():
    # Blenderにクラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)

    #メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    #3Dビューに描画関数を追加
    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(DrawCollider.draw_collider, (), "WINDOW", "POST_VIEW")
    print("レベルエディタが有効化されました。")


#Add-On無効化時コールバック
def unregister():
    #メニューから項目を削除
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    #3Dビューから描画関数を削除
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle, "WINDOW")
    
    # Blenderから項目を削除
    for cls in classes:
        bpy.utils.unregister_class(cls)
    print("レベルエディタが無効化されました。")

# Blenderに登録するクラスリスト
classes = (
    TOPBAR_MT_my_menu,
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_spher,
    MYADDON_OT_export_scene,
    MYADDON_OT_add_filename,
    MYADDON_OT_add_collider,
    OBJECT_PT_file_name,
    OBJECT_PT_collider,
)

if __name__ == "__main__":
    register()
