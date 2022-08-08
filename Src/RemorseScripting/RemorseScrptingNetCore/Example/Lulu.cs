using System;

using RemorseEngine;

public class Lulu : DotNetBehaviour
{
	public Lulu(IntPtr pGo, int length) : base(pGo, length) { }
	public static void Awake(IntPtr pGo, int length)
	{ Init( new Lulu(pGo, length) ); }
	
	// Called Start
	public static void Start()
	{
		Debug.Log($"This Start LULU named {gameObject.GetName()}");	
	}
	
	// Called Every Frame
	public static void Update()
	{
		Debug.Log($"This UPDATE---- LULU named {go.GetName()}");
		
	}
}
