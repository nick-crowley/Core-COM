#pragma once
#include "comFramework.h"

namespace detail
{
	template <typename ReturnType, typename ParamTuple, size_t... Idx>
	struct FunctionSignature
	{
		constexpr 
		FunctionSignature() noexcept = default;
	};
	
	template <typename T>
	bool constexpr is_method_v = false;
	
	template <typename CallableTarget, typename ParamTuple, size_t... Idx>
	class CallAdapter0
	{
		CallableTarget	Callable;

	public:
		constexpr 
		CallAdapter0(CallableTarget method, FunctionSignature<void,ParamTuple,Idx...>) noexcept
			: Callable(std::move(method)) 
		{}

		void
		operator()(std::tuple_element_t<Idx,ParamTuple>... args) const requires(!is_method_v<CallableTarget>)
		{
			this->Callable(args...);
		}
			
		template <typename CoClass> requires(is_method_v<CallableTarget>)
		void
		operator()(CoClass& object, std::tuple_element_t<Idx,ParamTuple>... args) const 
		{
			this->Callable(object, args...);
		}
	};
	
	template <typename CallableTarget, typename Result, typename ParamTuple, size_t... Idx>
	class CallAdapter1
	{
		CallableTarget	Callable;

	public:
		constexpr 
		CallAdapter1(CallableTarget method, FunctionSignature<Result,ParamTuple,Idx...>) noexcept
			: Callable(std::move(method)) 
		{}
		
		Result
		operator()(std::tuple_element_t<Idx,ParamTuple>... args) const requires(!is_method_v<CallableTarget>)
		{
			Result  retval {};
			this->Callable(args..., &retval);
			return std::move(retval);
		}

		template <typename CoClass> requires(is_method_v<CallableTarget>)
		Result
		operator()(CoClass& object, std::tuple_element_t<Idx,ParamTuple>... args) const
		{
			Result  retval {};
			this->Callable(object, args..., &retval);
			return std::move(retval);
		}
	};
	
	template <typename CallableTarget, typename ResultTuple, typename ParamTuple, size_t... Idx>
	class CallAdapterN
	{
		CallableTarget	Callable;

	public:
		constexpr 
		CallAdapterN(CallableTarget method, FunctionSignature<ResultTuple,ParamTuple,Idx...>) noexcept
			: Callable(std::move(method)) 
		{}

	public:
		ResultTuple
		operator()(std::tuple_element_t<Idx,ParamTuple>... args) const requires(!is_method_v<CallableTarget>)
		{
			return this->call(std::make_index_sequence<std::tuple_size_v<ResultTuple>>{}, args...);
		}

		template <typename CoClass> requires(is_method_v<CallableTarget>)
		ResultTuple
		operator()(CoClass& object, std::tuple_element_t<Idx,ParamTuple>... args) const
		{
			return this->call(object, std::make_index_sequence<std::tuple_size_v<ResultTuple>>{}, args...);
		}

	private:
		template <size_t... ResultIdx> requires(!is_method_v<CallableTarget>)
		ResultTuple
		call(std::index_sequence<ResultIdx...>, std::tuple_element_t<Idx,ParamTuple>... args) const 
		{
			ResultTuple  retvals;
			this->Callable(args..., std::addressof(std::get<ResultIdx>(retvals))...);
			return std::move(retvals);
		}

		template <typename CoClass, size_t... ResultIdx> requires(is_method_v<CallableTarget>)
		ResultTuple
		call(CoClass& object, std::index_sequence<ResultIdx...>, std::tuple_element_t<Idx,ParamTuple>... args) const 
		{
			ResultTuple  retvals;
			this->Callable(object, args..., std::addressof(std::get<ResultIdx>(retvals))...);
			return std::move(retvals);
		}
	};
		
	template <typename CallableTarget, typename... Parameters, size_t... Idx>
	auto constexpr
	makeCallAdapter0(CallableTarget fx, std::index_sequence<Idx...>) noexcept
	{
		using all_params_t = std::tuple<Parameters...>;

		return CallAdapter0{
			std::move(fx),
			FunctionSignature<
				void,all_params_t,Idx...
			>{}
		};
	}
		
	template <unsigned NumParams, typename CallableTarget, typename... Parameters, size_t... Idx>
	auto constexpr
	makeCallAdapter1(CallableTarget fx, std::index_sequence<Idx...>) noexcept
	{
		using all_params_t = std::tuple<Parameters...>;
		using result_param_t = meta::tuple_back_t<all_params_t>;
		using remaining_params_t = meta::tuple_first_n_t<NumParams-1,all_params_t>;
			
		return CallAdapter1{
			std::move(fx),
			FunctionSignature<
				std::remove_pointer_t<result_param_t>,remaining_params_t,Idx...
			>{}
		};
	}
		
	template <unsigned NumResults, unsigned NumParams, typename CallableTarget, typename... Parameters, size_t... Idx>
	auto constexpr
	makeCallAdapterN(CallableTarget fx, std::index_sequence<Idx...>) noexcept
	{
		using all_params_t = std::tuple<Parameters...>;
		using result_params_t = meta::tuple_last_n_t<NumResults,all_params_t>;
		using remaining_params_t = meta::tuple_first_n_t<NumParams-NumResults,all_params_t>;
		
		return CallAdapterN{
			std::move(fx), 
			FunctionSignature<
				meta::tuple_transform_t<result_params_t,std::remove_pointer_t>,remaining_params_t,Idx...
			>{}
		};
	}
	
	template <unsigned NumResults, unsigned NumParams, typename CallableTarget, typename... Parameters>
	auto constexpr 
	makeCallAdapter(CallableTarget fx) noexcept
	{
		if constexpr (NumResults == 0)
			return makeCallAdapter0<CallableTarget,Parameters...>(
				std::move(fx), std::make_index_sequence<NumParams>{}
			);
		else if constexpr (NumResults == 1)
			return makeCallAdapter1<NumParams,CallableTarget,Parameters...>(
				std::move(fx), std::make_index_sequence<NumParams-1>{}
			);
		else 
			return makeCallAdapterN<NumResults,NumParams,CallableTarget,Parameters...>(
				std::move(fx), std::make_index_sequence<NumParams-NumResults>{}
			);
	}
}  // namespace detail
