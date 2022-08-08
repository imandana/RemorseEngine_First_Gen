using System;
using System.Collection;
using System.Runtime.InteropServices;

namespace RemorseEngine
{
	[StructLayout(LayoutKind.Sequential)]
	public class GameObject
	{
		public IntPtr nativeName;
		public string name;
		public int posX;
		public int posY;
		
		private static GameObject staticGO;
		
		public void MoveTo(int x, int y)
		{
			staticGO = this;
			staticGO.posX = x ;
			staticGO.posY = y ;
		}
		
		public void ChangeName( string pName )
		{
			staticGO = this;
			staticGO.name = pName;
		}
		
		public string GetName()
		{
			return name;
		}
	}
	
	//// Managed Log
	public sealed class Debug
	{
		public static void Log(string str)
		{
			Console.WriteLine(str);
		}
	}
	
	//// Native Log
	//public void Log(string str);
	
	public class DotNetBehaviour
	{
		protected static DotNetBehaviour self;
		protected static GameObject gameObject;
		
		private static ArrayList<GameObject> gameObjects;
		private static ArrayList<DotNetBehaviour> our;
		int position;
		public DotNetBehaviour(IntPtr pGo, int length)
		{
			gameObject = Marshal.PtrToStructure<GameObject>(pGo);
		}

		protected static void Init(DotNetBehaviour obj)
        {
			self = obj;
		}
		
		public delegate void AwakeDelegate(IntPtr pGo, int length);
        public delegate void StartDelegate();
		public delegate void UpdateDelegate();
		public delegate void FixedUpdateDelegate();
		
		
		public delegate IntPtr GetStrManagedDelegate();
	}
}
