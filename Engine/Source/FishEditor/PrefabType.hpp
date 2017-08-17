#pragma once

namespace UnityEditor
{
	/// <summary>
	///   <para>The type of a prefab object as returned by PrefabUtility.GetPrefabType.</para>
	/// </summary>
	enum class  PrefabType
	{
		/// <summary>
		///   <para>The object is not a prefab nor an instance of a prefab.</para>
		/// </summary>
		None,
		/// <summary>
		///   <para>The object is a user created prefab asset.</para>
		/// </summary>
		Prefab,
		/// <summary>
		///   <para>The object is an imported 3D model asset.</para>
		/// </summary>
		ModelPrefab,
		/// <summary>
		///   <para>The object is an instance of a user created prefab.</para>
		/// </summary>
		PrefabInstance,
		/// <summary>
		///   <para>The object is an instance of an imported 3D model.</para>
		/// </summary>
		ModelPrefabInstance,
		/// <summary>
		///   <para>The object was an instance of a prefab, but the original prefab could not be found.</para>
		/// </summary>
		MissingPrefabInstance,
		/// <summary>
		///   <para>The object is an instance of a user created prefab, but the connection is broken.</para>
		/// </summary>
		DisconnectedPrefabInstance,
		/// <summary>
		///   <para>The object is an instance of an imported 3D model, but the connection is broken.</para>
		/// </summary>
		DisconnectedModelPrefabInstance
	}; 
}
