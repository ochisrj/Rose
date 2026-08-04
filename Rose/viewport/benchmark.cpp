#include "bechmark.h"
#include "imgui.h"

bool BenchMark::Visible = true;

void BenchMark::DrawWindow()
{
	if (!Visible) return;

	if (ImGui::Begin("Benchmark", &Visible))
	{
		ImGui::Text("Framerate : %.1f fps", ImGui::GetIO().Framerate);
		ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);

		ImGui::Separator();
		ImGui::Text("Draw Calls  : %d", ImGui::GetDrawData() ? ImGui::GetDrawData()->CmdListsCount : 0);
		ImGui::Text("Total Verts : %d", ImGui::GetDrawData() ? ImGui::GetDrawData()->TotalVtxCount : 0);
	}
	ImGui::End();
}
