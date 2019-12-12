{ // Update Final Mat
	bool dirty_mat = false;
	if(this->mIsFigureDirty)
	{
		this->_updateFigureMat();
		this->mIsFigureDirty = false;
		dirty_mat = true;
	}

	if(mParent)
	{
		auto pv = mParent->_getMatVersion();
		if(pv != mParentMatVersion)
		{
			mParentMatVersion = pv;
			dirty_mat = true;
		}
	}
	if(dirty_mat)
	{
		if(mParent && mParent->_getMat())
		{
			mFinalMat = (*mParent->_getMat() & mFigureMat);
		}
		else
		{
			mFinalMat = mFigureMat;
		}
	}
}
