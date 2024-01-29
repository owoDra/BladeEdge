// Copyright (C) 2024 owoDra

#include "BELoadoutRequest.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELoadoutRequest)


bool FBELoadoutRequest::IsValid() const
{
	return !ItemDatas.IsEmpty();
}
