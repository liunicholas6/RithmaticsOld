using Godot;
using System;

public class Singleton : Node
{
    private static bool _debug = false;
    private static Color _chalkColor = new Color(1, 1, 1);
    public static bool Debug{
        get => _debug;
        set => _debug = value;
    }
    public static Color ChalkColor{
        get => _chalkColor;
    }
}
