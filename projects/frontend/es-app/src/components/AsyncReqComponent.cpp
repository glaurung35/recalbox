#include "components/AsyncReqComponent.h"
#include "Renderer.h"

AsyncReqComponent::AsyncReqComponent(Window* window, std::shared_ptr<HttpReq> req, std::function<void(std::shared_ptr<HttpReq>)> onSuccess, std::function<void()> onCancel) 
	: GuiComponent(window), 
	mSuccessFunc(std::move(onSuccess)), mCancelFunc(std::move(onCancel)), mTime(0), mRequest(std::move(req))
{
	
}

bool AsyncReqComponent::input(InputConfig* config, Input input)
{
	if(input.value != 0 && config->isMappedTo("a", input))
	{
		if(mCancelFunc)
			mCancelFunc();

		delete this;
	}

	return true;
}

void AsyncReqComponent::update(int deltaTime)
{
	if(mRequest->status() != HttpReq::REQ_IN_PROGRESS)
	{
		mSuccessFunc(mRequest);
		delete this;
		return;
	}

	mTime += deltaTime;
}

void AsyncReqComponent::render(const Transform4x4f& parentTrans)
{
	(void)parentTrans;

	Transform4x4f trans = Transform4x4f::Identity();
	trans = trans.translate(Vector3f((float)Renderer::getScreenWidth() / 2.0f, (float)Renderer::getScreenHeight() / 2.0f, 0));
	Renderer::setMatrix(trans);

	Vector3f point(cos((float)mTime * 0.01f) * 12, sin((float)mTime * 0.01f) * 12, 0);
	Renderer::drawRect((int)point.x(), (int)point.y(), 8, 8, 0x0000FFFF);
}

std::vector<HelpPrompt> AsyncReqComponent::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("a", "cancel"));
	return prompts;
}
