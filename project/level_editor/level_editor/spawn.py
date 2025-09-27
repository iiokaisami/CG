import bpy
import os
import bpy.ops

#オペレータ 出現ポイントのシンボルを読み込む
class MYADDON_OT_spawn_import_symbol(bpy.types.Operator):
    bl_idname = "myaddon.spawn_import_symbol"
    bl_label = "出現ポイントシンボルのインポート"
    bl_description = "出現ポイントのシンボルをインポートします"

    prototype_object_name = "PrototypePlayerSpawn"
    object_name = "PlayerSpawn"

    def load_obj(self,cantext):
        print("出現ポイントのシンボルをインポートします")
        
        # 重複ロード防止
        spawn_object = bpy.data.objects.get(SpawnNames.names[cantext][SpawnNames.PROTOTYPE])
        if spawn_object is not None:
            return {'CANCELLED'}

        # スクリプトが配置されているディレクトリの名前を所得する
        addon_dirextory = os.path.dirname(__file__)
        # ディレクトリからのモデルファイルの相対パスを記述
        relative_path = SpawnNames.names[cantext][SpawnNames.FILENAME]
        # 合成してモデルファイルのフルパスを得る
        full_path = os.path.join(addon_dirextory,relative_path)

        # オブジェクトをインポート
        bpy.ops.wm.obj_import('EXEC_DEFAULT', 
                              filepath = full_path, display_type = 'THUMBNAIL',
                              forward_axis = 'Z',up_axis = 'Y')
        # 回転を適応
        bpy.ops.object.transform_apply(location = False,
                                        rotation = True, scale = False, properties = False,
                                        isolate_users = False)
        
        # アクティブなオブジェクトを取得
        object = bpy.context.active_object
        # オブジェクト名を変更
        object.name = SpawnNames.names[cantext][SpawnNames.PROTOTYPE]
        # オブジェクトの種類の設定
        object["type"] = SpawnNames.names[cantext][SpawnNames.INSTANCE]

        # メモリ上にはおいておくがシーンから外す
        bpy.context.collection.objects.unlink(object)

        return{'FINISHED'}
    
    def execute(self,context):
        #Enemy
        self.load_obj("Enemy")
        #Player
        self.load_obj("Player")

        return{'FINISHED'}
    
# オペレータ 出現ポイントのシンボルを作成・配置する
class MYADDON_OT_spawn_create_symbol(bpy.types.Operator):
    bl_idname = "myaddon.create_spawn_symbol"
    bl_label = "出現ポイントのシンボルの作成"
    bl_description = "出現ポイントのシンボルを作成します"
    bl_options = {'REGISTER','UNDO'}

    # プロパティ(引数として渡せる)
    type: bpy.props.StringProperty(name="Type",default="Player")

    def execute(self, context):
        # 読み込み済みのコピー元オブジェクトを検索
        spawn_object = bpy.data.objects.get(SpawnNames.names[self.type][SpawnNames.PROTOTYPE])
        
        # まだ読み込んでいない場合
        if spawn_object is None:
            # 読み込みオペレータを実行する
            bpy.ops.myaddon.spawn_import_symbol('EXEC_DEFAULT')
            # 再検索
            spawn_object = bpy.data.objects.get(SpawnNames.names[self.type][SpawnNames.PROTOTYPE])
        
        print("出現ポイントのシンボルを作成します")
        # Blenderでの選択を解除する
        bpy.ops.object.select_all(action = 'DESELECT')
        
        # 複製元の非表示オブジェクトを複製する
        object = spawn_object.copy()

        # 複製したオブジェクトを現在のシーンにリンク(出現させる)
        bpy.context.collection.objects.link(object)

        # オブジェクト名を変更
        object.name = SpawnNames.names[self.type][SpawnNames.INSTANCE]

        return {'FINISHED'}
    
class MYADDON_OT_spawn_create_player_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_spawn_create_player_symbol"
    bl_label = "プレイヤー出現シンボルの作成"
    bl_description = "プレイヤー出現ポイントのシンボルを作成します"

    def execute(self,context):

        bpy.ops.myaddon.create_spawn_symbol('EXEC_DEFAULT',type = "Player")

        return{'FINISHED'}

class MYADDON_OT_spawn_create_enemy_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_spawn_create_enemy_symbol"
    bl_label = "敵出現シンボルの作成"
    bl_description = "敵出現ポイントのシンボルを作成します"

    def execute(self,context):

        bpy.ops.myaddon.create_spawn_symbol('EXEC_DEFAULT',type = "Enemy")

        return{'FINISHED'}

class SpawnNames():

    #インデックス
    PROTOTYPE = 0 # プロトタイプのオブジェクト名
    INSTANCE = 1  # 量産時のオブジェクト名
    FILENAME = 2  # リソースファイル名

    names = {}
    # name["キー"] = {プロトタイプのオブジェクト名、量産時のオブジェクト名、リソースファイル名}
    names["Enemy"] = ("PrototypeEnemySpawn","EnemySpawn","enemy/enemy.obj")
    names["Player"] = ("PrototypePlayerSpawn","PlayerSpawn","player/player.obj")
