namespace FishEngine::Serialization
{
	template <class T, class OutputArchive>
	struct is_output_serializable : std::integral_constant<bool,
		detail::count_output_serializers<T, OutputArchive>::value == 1> {};
}