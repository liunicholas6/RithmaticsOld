//Surface.cs contains information needed by rithmatic lines of all types (drawn, true, and/or special).

using Godot;
using System;
using System.Collections.Generic;

//Code for the surface upon which all rithmatic lines reside. Doesn't really do much of anything as of now.
public class Surface : Node2D{ }

#region Rithmatic Line Base Classes
public class Warding : Godot.RigidBody2D{
    protected Vector2[] _input;
    protected float _radius = 0;
    private List<SegmentInfo> _segInfo;
    private CollisionShape2D collision;
    private KinematicBody2D _filledBody;
    protected float _damageFactor = (float) 0.5;    

    public override void _Ready(){
        PhysicsMaterialOverride = (PhysicsMaterial) ResourceLoader.Load("res://Main/MainSurfaceScripts/WardingMat.tres");
        ContinuousCd = (CCDMode) 1;
        Mass = _radius / 100;
        Inertia =  Mass * (float) Math.Pow(_radius, 2);
        _segInfo =  new List<SegmentInfo>();
        collision = new CollisionShape2D();
        AddChild(collision);
        CreateShapeOwner(collision);
        for (int i = 0; i<_input.Length-1; i++){
            Vector2 a = _input[i];
            Vector2 b = _input[i+1];
            _segInfo.Add(new SegmentInfo(a, b));
            SegmentShape2D shape = new SegmentShape2D();
            shape.A = a;
            shape.B = b;
            ShapeOwnerAddShape(0, shape);
        }
    }
    
    public void TakeDamage(KinematicCollision2D col, Vigor vigor){
        Vector2 impulse = 300 * vigor.PushFactor * (float) Math.Abs(Math.Cos(vigor.Dir.AngleTo(Position-col.Position))) * vigor.Dir;
        ApplyImpulse((col.Position - Position).Rotated(-Rotation), impulse);
        int destroyedSegs = 0;
        for(int i = col.ColliderShapeIndex-3; i<col.ColliderShapeIndex+3; i++){
            if (TakeDamageHelper(i-destroyedSegs % _segInfo.Count, vigor.DamageFactor * _damageFactor / 3)) destroyedSegs++;
        }
        Update();
    }

    private bool TakeDamageHelper(int index, float damage){
        if (_segInfo[index].dealDamage(damage)){
            _segInfo.RemoveAt(index);
            ShapeOwnerRemoveShape(0, index);
            return true;
        }
        return false;
    }
    public override void _Draw(){
        foreach (SegmentInfo info in _segInfo){
            Color color = Singleton.ChalkColor;
            color.a = (float) info.HitPoints;
            DrawLine(info.A, info.B, color, 5);
        }
    }
}


public class Forbiddance : StaticBody2D{
    protected Vector2[] _input;
    protected Vector2 _dir;
    protected float _damageFactor = 1;
    private List<SegmentInfo> _segInfo;
    private CollisionShape2D collision;

    public Vector2 Dir{
        get => _dir;

    }    public override void _Ready(){
        _segInfo =  new List<SegmentInfo>();
        collision = new CollisionShape2D();
        AddChild(collision);
        CreateShapeOwner(collision);
        for (int i = 0; i<_input.Length-1; i++){
            Vector2 a = _input[i];
            Vector2 b = _input[i+1];
            _segInfo.Add(new SegmentInfo(a, b));
            SegmentShape2D shape = new SegmentShape2D();
            shape.A = a;
            shape.B = b;
            ShapeOwnerAddShape(0, shape);
        }
    }

    public void TakeDamage(int index, float power){
        int destroyedSegs = 0;
        for(int i = index-3; i<index+3; i++){
            if(_segInfo.Count>i && i>=0) if(TakeDamageHelper(i-destroyedSegs, power * _damageFactor)) destroyedSegs++;
        }
        Update();
    }

    private bool TakeDamageHelper(int index, float damage){
        if (_segInfo[index].dealDamage(damage)){
            _segInfo.RemoveAt(index);
            ShapeOwnerRemoveShape(0, index);
            return true;
        }
        return false;
    }
    public override void _Draw(){
        foreach (SegmentInfo info in _segInfo){
            Color color = Singleton.ChalkColor;
            color.a = (float) info.HitPoints;
            DrawLine(info.A, info.B, color, 5);
        }
    }
}


//Base class for lines of vigor
public class Vigor : Node2D{
    protected Vector2[] _visible;
    protected Vector2 _dir;
    protected float _amp = 0;
    protected float _omega = 0;
    protected float _phase = 0;
    protected float _shift = 0;
    protected bool _unformed = true;
    private bool _destroyed = false;
    protected KinematicBody2D _vigorTip;
    protected float _damageFactor = (float) 0.5;
    protected float _pushFactor = (float) 0.5;

    public Vector2 Dir{
        get => _dir;
    }
    public float Amp{
        get => _amp;
    }
    public float Omega{
        get => _omega;
    }
    public Vector2[] Drawn{
        get => _visible;
    }

    public virtual int Count{
        get => _visible.Length;
    }

    public float DamageFactor{
        get => _damageFactor;
    }    
    public float PushFactor{
        get => _pushFactor;
    }

    protected Vector2 GetWavePoint(float x){
        return new Vector2(x, _amp * (float) Math.Cos(_omega * x - _phase) + _shift);
    }

    public override void _PhysicsProcess(float delta){
        if (_destroyed){
            Vector2[] temp = new Vector2[_visible.Length-1];
            for (int i = 0; i < temp.Length; i++){
                temp[i] = _visible[i];
            }
            _visible = temp;
            if (_visible.Length == 0) QueueFree();
            Update();
            return;
        }
        if (_unformed) Form(delta);
        if (_vigorTip != null){
            Vector2 nextPoint = GetWavePoint(_vigorTip.Position.x+delta*15/_omega);
            Vector2 velocity = (nextPoint-_vigorTip.Position).Rotated(_dir.Angle());
            ManageCollision(_vigorTip.MoveAndCollide(velocity, false));
                for (int i = _visible.Length-1; i>0; i--){
                    _visible[i]=_visible[i-1];
                }
            _visible[0] = nextPoint;
        }
        Update();
    }

    protected virtual void Form(float delta){
        ;
    }

    public void MakeTip(){
        CapsuleShape2D shape = new CapsuleShape2D();
        shape.Height = 0;
        shape.Radius = (float) 2;
        CollisionShape2D collision = new CollisionShape2D();
        collision.Shape = shape;
        _vigorTip = new KinematicBody2D();
        _vigorTip.AddChild(collision);
        _vigorTip.Position = _visible[0];
        AddChild(_vigorTip);
    }

    protected virtual void ManageCollision(KinematicCollision2D col){
        if (col == null) return;
        if (col.Collider is Warding){
            ((Warding)col.Collider).TakeDamage(col, this);
            QueueFree();
        }
        if (col.Collider is DrawnForbiddance){
            _vigorTip.QueueFree();
            ((DrawnForbiddance)col.Collider).TakeDamage(col.ColliderShapeIndex, _damageFactor);
            GetParent().AddChild(new ReflectedVigor(col, this));
            _destroyed = true;
        }
    }
    public override void _Draw(){
        if (_visible != null && _visible.Length >= 2) DrawPolyline(_visible, Singleton.ChalkColor, 5);
    }
}
#endregion

#region Variant Rithmatic Lines
public class ReflectedVigor: Vigor{
    private Godot.Object _collider;
    private int _colliderIndex;
    private int _count;
    private int _pointsMade = 0;
    private List<Vector2> _drawnList = new List<Vector2>();
    private int _ghost = 0;
    public override int Count{
        get => _count;
    }
    public ReflectedVigor(KinematicCollision2D col, Vigor original){
        GD.Print("Reflection Happened");
        Position = col.Position;
        _collider = col.Collider;
        _colliderIndex = col.ColliderShapeIndex;
        _dir = original.Dir.Reflect(((Forbiddance)(col.Collider)).Dir);
        Rotation = _dir.Angle();
        _amp = original.Amp;
        _omega = original.Omega;
        _count = original.Count;
        _phase = (float) Math.PI/2;
        //if (original.Drawn[0].y>original.Drawn[1].y) _phase += (float) Math.PI;
    }
    protected override void Form(float delta){
        _drawnList.Insert(0, GetWavePoint(_pointsMade*delta*15/_omega));
        _visible = _drawnList.ToArray();
        _pointsMade++;
        if(_pointsMade == 2) MakeTip();
        if(_ghost > 0){
            _ghost--;
            if(_ghost == 0){
                _vigorTip.CollisionLayer = 1;
                _vigorTip.CollisionMask = 1;
            }
        }
        if(_pointsMade == _count) _unformed = false;
    }
    protected override void ManageCollision(KinematicCollision2D col){
        if(col == null) return;
        if(col.Collider == _collider && col.ColliderShapeIndex == _colliderIndex){
            _vigorTip.CollisionLayer = 0;
            _vigorTip.CollisionMask = 0;
            _ghost = 3;
            return;
        }
        base.ManageCollision(col);
    }
}
#endregion

#region Helper Classes
public class SegmentInfo{
    private Vector2 _a;
    private Vector2 _b; 
    private float _hitPoints = 1;
    public SegmentInfo(Vector2 a, Vector2 b){
        _a = a;
        _b = b;
    }
    public Vector2 A{
        get => _a;
    }
    public Vector2 B{
        get => _b;
    }
    public float HitPoints{
        get => _hitPoints;
    }
    public bool dealDamage(float damage){
        if (_hitPoints < damage){
            _hitPoints = 0;
            return true;
        }
        _hitPoints -= damage;
        return false;
    }
}
#endregion



